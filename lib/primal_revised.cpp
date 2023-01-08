#include "primal_revised.h"
#include "constraint.h"
#include "simplex_common.h"

#include "variable.h"

#include "lu_factor.h"
#include "solve_error.h"

#include "matrix.h"
#include "tools.h"

#include "logging.h"

#include <algorithm>

namespace jsolve
{
namespace
{
struct Parameters
{
    int refactor_iter{100}; // Periodically recompute LU factorisation
    int max_iter{10000};    // Stopping criteria - max simplex iterations
    Number EPS1{1e-8};      // Minimum value to consider as exiting var
    Number EPS2{1e-5};      // Protection from division by zero
};

struct SolveData
{
    // Everything needed to do iterations of the revised simplex algorithm.
    // Using notation from 'Linear Programming' (Vanderbei, 2020) p102.
    Mat A;
    Mat c;
    Mat B;
    Mat N;
    Mat x_basic;
    Mat z_non_basic;
    std::vector<VarData> basics;
    std::vector<VarData> non_basics;
    int n_iter{0};
};

std::optional<std::size_t> choose_entering(const Mat& column, Number EPS2)
{
    // Choses most negative variable in the column vector.

    std::optional<std::size_t> entering;
    Number current_min{-EPS2};

    for (std::size_t curr_idx = 0; curr_idx < column.n_rows(); curr_idx++)
    {
        if (column(curr_idx, 0) < current_min)
        {
            entering = curr_idx;
            current_min = column(curr_idx, 0);
        }
    }

    return entering;
}

std::optional<std::size_t> choose_leaving(const Mat& num, const Mat& denom, Number EPS1)
{
    // Calculates argmin(num/denom), where denom > zero.

    std::optional<std::size_t> leaving;
    Mat::value_type min_ratio{std::numeric_limits<Mat::value_type>::max()};

    for (std::size_t curr_idx = 0; curr_idx < num.n_rows(); curr_idx++)
    {
        if (denom(curr_idx, 0) > EPS1)
        {
            auto curr_ratio{num(curr_idx, 0) / denom(curr_idx, 0)};

            if (curr_ratio < min_ratio)
            {
                min_ratio = curr_ratio;
                leaving = curr_idx;
            }
        }
    }

    return leaving;
}

Number calc_primal_obj(const Mat& c, const Mat& x_basic, const std::vector<VarData>& basics)
{
    Number primal_obj{0.0};

    for (std::size_t curr_idx = 0; curr_idx < x_basic.n_rows(); curr_idx++)
    {
        primal_obj += c(basics[curr_idx].index, 0) * x_basic(curr_idx, 0);
    }

    return primal_obj;
}

void log_iteration(int iter, const SolveData& data)
{
    int log_every{1};

    auto sum_if_negative = [](const auto& sum, const auto& current) {
        return current < 0 ? sum + current : sum;
    };

    auto primal_obj{calc_primal_obj(data.c, data.x_basic, data.basics)};
    auto dual_infeas = std::accumulate(std::begin(data.z_non_basic), std::end(data.z_non_basic), 0.0, sum_if_negative);
    auto primal_infeas = std::accumulate(std::begin(data.x_basic), std::end(data.x_basic), 0.0, sum_if_negative);

    std::string progress{fmt::format(
        "It {:6} Obj {:12.4f} DInf: {:12.4f} PInf: {:12.4f}", iter, primal_obj, dual_infeas, primal_infeas
    )};

    if (iter % log_every == 0)
    {
        log()->info(progress);
    }
    else
    {
        log()->debug(progress);
    }
} // namespace

SolveData init_data(const Model& model)
{
    // Setup the variables needed to conduct iterations of the revised simplex algorithm.

    auto num_vars = model.get_variables().size();
    auto num_constr = model.get_constraints().size();

    auto n = num_vars;
    auto m = num_constr;

    // Populate A matrix

    Mat A{m, n, 0.0};
    for (const auto& [n_cons, cons_pair] : enumerate(model.get_constraints()))
    {
        assert(cons_pair.second->type() == Constraint::Type::EQUAL);
        for (const auto& [n_var, var_pair] : enumerate(model.get_variables()))
        {
            auto found_entry = cons_pair.second->get_entries().find(var_pair.second.get());
            if (found_entry != std::end(cons_pair.second->get_entries()))
            {
                A(n_cons, n_var) = found_entry->second;
            }
        }
    }

    log()->trace(A);

    // Create c column vector = [c]
    Mat c{n, 1, 0.0};
    for (const auto& [n_var, pair] : enumerate(model.get_variables()))
    {
        c(n_var, 0) = pair.second->cost();
    }

    if (model.sense() == Model::Sense::MIN)
    {
        c *= -1;
    }

    log()->trace(c);

    // Create b (RHS) vector
    Mat b{m, 1, 0.0};
    for (const auto& [n_cons, pair] : enumerate(model.get_constraints()))
    {
        assert(pair.second->type() == Constraint::Type::EQUAL);
        b(n_cons, 0) = pair.second->rhs();
    }

    log()->trace(b);

    // Find the initial basis

    std::size_t basis_size = std::ranges::count_if(model.get_variables(), [](const auto& pair) {
        return pair.second->slack() || pair.second->artifical();
    });

    std::vector<VarData> basics;
    basics.reserve(basis_size);
    std::vector<VarData> non_basics;
    non_basics.reserve(n - basis_size);

    Mat B{m, basis_size};
    Mat N{m, n - basis_size};

    Mat x_basic{b};
    Mat z_non_basic{n - basis_size, 1};

    // TODO - Clean this up with std::mdspan
    for (const auto& [n_var, var_pair] : enumerate(model.get_variables()))
    {
        auto index = static_cast<int>(n_var);
        if (var_pair.second->artifical())
        {
            basics.push_back({index, index, true, true});
            B.update({}, {basics.size() - 1}, A.slice({}, {n_var}));
        }
        else if (var_pair.second->slack())
        {
            basics.push_back({index, index, true, false});
            B.update({}, {basics.size() - 1}, A.slice({}, {n_var}));
        }
        else
        {
            non_basics.push_back({index, index, false, false});
            N.update({}, {non_basics.size() - 1}, A.slice({}, {n_var}));
            z_non_basic(non_basics.size() - 1, 0) = -1.0 * c(n_var, 0);
        }
    }

    // For the simplex we need:

    log()->trace(B);
    log()->trace(N);

    log()->trace(x_basic);
    log()->trace(z_non_basic);

    return {A, c, B, N, x_basic, z_non_basic, basics, non_basics};
}

Mat eta_inverse_solve(const Mat& u, const Mat& dx, std::size_t i)
{
    // Solves Eta * y = u for y using the the formula from 'Linear Programming' (Vanderbei, 2020) p135.
    // TODO: Avoid creation of e

    auto e = Mat(dx.n_rows(), 1);
    e(i, 0) = 1;
    auto y = u - (u(i, 0) / dx(i, 0)) * (dx - e);
    return y;
};

Mat eta_inverse_transpose_solve(const Mat& u, const Mat& dx, std::size_t i)
{
    // Solves trans(Eta) * y = u for y using the formula from 'Linear Programming' (Vanderbei, 2020) p136.
    // TODO: Avoid creation of e

    auto e = Mat(dx.n_rows(), 1);
    e(i, 0) = 1;
    auto y = u - e * ((dx - e).make_transpose() * u) / dx(i, 0);
    return y;
};

Mat ftran(const lu_result<Number>& lu, const Mat& b, const std::vector<std::pair<Mat, std::size_t>>& etas)
{
    // Implement FTRAN using the eta matrix factorisation of the basis, plus the initial LU factorisation.
    // This is comination of the implementations from:
    // 'Linear Programming' (Vanderbei, 2020) p133.
    // 'Linear Programming' (Chvatal, 1983) p109.

    // Use the LU factorisation in the first iteration.
    auto dx = lu_solve(lu.L, lu.U, lu.perm, b);

    // Apply etas recursively to update dx.
    for (const auto& [eta, leaving] : etas)
    {
        dx = eta_inverse_solve(dx, eta, leaving);
    }

    return dx;
}

Mat btran(const lu_result<Number>& lu, const Mat& b, const std::vector<std::pair<Mat, std::size_t>>& etas)
{
    // Implement BTRAN using the eta matrix factorisation of the basis, plus the initial LU factorisation.
    // This is comination of the implementations from:
    // 'Linear Programming' (Vanderbei, 2020) p133.
    // 'Linear Programming' (Chvatal, 1983) p109.

    auto u = b;

    // Apply etas recursively in reverse to update u.
    auto it = std::rbegin(etas);
    while (it != std::rend(etas))
    {
        u = eta_inverse_transpose_solve(u, it->first, it->second);
        ++it;
    }

    // Use the LU factorisation
    auto v = lu_transpose_solve(lu.L, lu.U, lu.perm, u);

    return v;
}

bool solve_primal(SolveData& data, Parameters params)
{
    // Solve using the primal (revised) simplex algorithm.
    // Uses implementation from 'Linear Programming' (Vanderbei, 2020) p102.
    // Returns true if a solution is present.

    Mat& A = data.A;
    Mat& B = data.B;
    Mat& N = data.N;
    Mat& x_basic = data.x_basic;
    Mat& z_non_basic = data.z_non_basic;
    std::vector<VarData>& basics = data.basics;
    std::vector<VarData>& non_basics = data.non_basics;
    int& iter = data.n_iter;

    // Store dx and the leaving index each iteration
    std::vector<std::pair<Mat, std::size_t>> etas;

    // Intial LU factorisation
    auto lu_current{jsolve::lu_factor(B)};
    int refactor_age{0};

    while (iter <= params.max_iter)
    {
        iter++;
        log_iteration(iter, data);

        if (refactor_age >= params.refactor_iter)
        {
            // Recompute LU factorisation of basis
            log()->info("Re-factoring basis...");
            lu_current = jsolve::lu_factor(B);
            etas.clear();
            refactor_age = 0;
        }
        else
        {
            refactor_age++;
        }

        // 1. Check optimality
        // 2. Find entering variable
        // Pick minimum (and negative) z_non_basic

        std::optional<std::size_t> entering = choose_entering(z_non_basic, params.EPS2);

        if (!entering)
        {
            // Optimal
            return true;
        }

        // 3. Calculate dx (FTRAN)
        auto dx = ftran(lu_current, N.slice({}, entering.value()), etas);

        // 4. Find the leaving variable
        std::optional<std::size_t> leaving = choose_leaving(x_basic, dx, params.EPS1);

        if (!leaving)
        {
            // Unbounded
            log()->warn("Unbounded");
            return false;
        }

        log()->debug("Entering: {} Leaving: {}", entering.value(), leaving.value());

        // 5. Calculate primal step length
        // t = x/dx

        auto t = x_basic(leaving.value(), 0) / dx(leaving.value(), 0);

        // 6. Calculate dz (BTRAN)
        auto ei = Mat{B.n_rows(), 1};
        ei(leaving.value(), 0) = 1;

        auto dz = -1.0 * N.make_transpose() * btran(lu_current, ei, etas);

        // 7. Calculate dual step lengths
        // s = z/dz (j)
        auto s = z_non_basic(entering.value(), 0) / dz(entering.value(), 0);

        // 8. Update primal and dual solutions
        x_basic = x_basic - t * dx;
        x_basic(leaving.value(), 0) = t;

        z_non_basic = z_non_basic - s * dz;
        z_non_basic(entering.value(), 0) = s;

        // 9. Update variables
        B.update({}, {leaving.value()}, A.slice({}, static_cast<std::size_t>(non_basics[entering.value()].index)));
        N.update({}, {entering.value()}, A.slice({}, static_cast<std::size_t>(basics[leaving.value()].index)));
        std::swap(basics[leaving.value()], non_basics[entering.value()]);

        // Save eta data
        etas.push_back({dx, leaving.value()});

        log()->trace(B);
        log()->trace(N);
    }

    if (iter >= params.max_iter)
    {
        log()->warn("Iteration limit ({}) reached", iter);
        return false;
    }

    log()->info("Optimal solution found");
    return true;
}

bool solve_dual(SolveData& data, Parameters params)
{
    // Solve using the dual (revised) simplex algorithm.
    // Uses implementation from 'Linear Programming' (Vanderbei, 2020) p102.
    // Returns true if a solution is present.

    Mat& A = data.A;
    Mat& B = data.B;
    Mat& N = data.N;
    Mat& x_basic = data.x_basic;
    Mat& z_non_basic = data.z_non_basic;
    std::vector<VarData>& basics = data.basics;
    std::vector<VarData>& non_basics = data.non_basics;
    int& iter = data.n_iter;

    // Store dx and the leaving index each iteration
    std::vector<std::pair<Mat, std::size_t>> etas;

    // Intial LU factorisation
    auto lu_current{jsolve::lu_factor(B)};
    int refactor_age{0};

    while (iter <= params.max_iter)
    {
        iter++;
        log_iteration(iter, data);

        if (refactor_age >= params.refactor_iter)
        {
            // Recompute LU factorisation of basis
            log()->info("Re-factoring basis...");
            lu_current = jsolve::lu_factor(B);
            etas.clear();
            refactor_age = 0;
        }
        else
        {
            refactor_age++;
        }

        // 1. Check optimality
        // 2. Find entering variable
        // Pick minimum (and negative) x_basic

        std::optional<std::size_t> entering = choose_entering(x_basic, params.EPS2);

        if (!entering)
        {
            // Optimal
            return true;
        }

        // 3. Calculate dz (BTRAN)
        auto ei = Mat{B.n_rows(), 1};
        ei(entering.value(), 0) = 1;

        auto dz = -1.0 * N.make_transpose() * btran(lu_current, ei, etas);

        log()->trace(dz);

        // 4. Find the leaving variable

        std::optional<std::size_t> leaving = choose_leaving(z_non_basic, dz, params.EPS1);

        if (!leaving)
        {
            // Unbounded
            log()->warn("Unbounded");
            return false;
        }

        // 5. Calculate dual step length
        // s = z/dz

        auto s = z_non_basic(leaving.value(), 0) / dz(leaving.value(), 0);

        // 6. Calculate dx (FTRAN)
        auto dx = ftran(lu_current, N.slice({}, {leaving.value()}), etas);

        log()->trace(dx);

        // 7. Calculate dual step lengths
        // t = x/dx (i)

        auto t = x_basic(entering.value(), 0) / dx(entering.value(), 0);

        // 8. Update primal and dual solutions

        x_basic = x_basic - t * dx;
        x_basic(entering.value(), 0) = t;

        z_non_basic = z_non_basic - s * dz;
        z_non_basic(leaving.value(), 0) = s;

        // 9. Update variables
        B.update({}, {entering.value()}, A.slice({}, static_cast<std::size_t>(non_basics[leaving.value()].index)));
        N.update({}, {leaving.value()}, A.slice({}, static_cast<std::size_t>(basics[entering.value()].index)));
        std::swap(basics[entering.value()], non_basics[leaving.value()]);

        // Save eta data
        etas.push_back({dx, entering.value()});

        log()->trace(B);
        log()->trace(N);
    }

    if (iter >= params.max_iter)
    {
        log()->warn("Iteration limit ({}) reached", iter);
        return false;
    }

    log()->info("Optimal solution found");
    return true;
}

Solution extract_solution(const Model& model, SolveData& data)
{
    // Extract solution from the solve data and put it into the original model.

    Solution sol{};

    auto primal = calc_primal_obj(data.c, data.x_basic, data.basics);

    sol.objective = model.sense() == Model::Sense::MIN ? -1.0 * primal : primal;

    for (std::size_t idx{0}; const auto& var_data : data.basics)
    {
        sol.variables[model.get_variable(var_data.index)->name()] = data.x_basic(idx, 0);
        ++idx;
    }

    for (const auto& var_data : data.non_basics)
    {
        sol.variables[model.get_variable(var_data.index)->name()] = 0;
    }

    return sol;
}

bool is_primal_feas(const SolveData& data)
{
    return data.x_basic.min() >= 0.0;
}

bool is_dual_feas(const SolveData& data)
{
    return data.z_non_basic.min() >= 0.0;
}

bool has_artificals_in_basis(const SolveData& data)
{
    return std::ranges::any_of(data.basics, [](const auto& var) { return var.dummy; });
}

void update_primal_objective(SolveData& data, const Mat& objective)
{
    // Change to a new primal objective and propagate to the dual variables.
    // So c becomes the new c, and the dual (z) variables are updated by:
    // z_n = transpose(inv(B)*N)*c_b - c_n

    data.c = objective;

    // Non-basics vars
    for (std::size_t idx{0}; const auto& var : data.non_basics)
    {
        data.z_non_basic(idx, 0) = -1.0 * data.c(var.index, 0);
        idx++;
    }

    // Basic vars
    auto c_b = Mat{data.basics.size(), 1};
    for (std::size_t idx{0}; const auto& var : data.basics)
    {
        c_b(idx, 0) = data.c(var.index, 0);
        idx++;
    }

    auto [L, U, perm] = jsolve::lu_factor(data.B.make_transpose());
    auto y = jsolve::forward_subs(L, c_b, perm);
    auto v = jsolve::backward_subs(U, y);

    auto N_t = data.N.make_transpose();
    data.z_non_basic.update({}, {}, data.z_non_basic + (N_t * v));
}

} // namespace

std::optional<Solution> solve_simplex_revised(const Model& model)
{
    // Setup
    SolveData data{init_data(model)};
    Parameters params{};

    bool has_solution{false};
    bool primal_feas{is_primal_feas(data)};
    bool dual_feas{is_dual_feas(data)};

    if (primal_feas && dual_feas)
    {
        log()->info("Starting basis is primal and dual feasible, already optimal");
        has_solution = true;
    }
    else if (primal_feas)
    {
        log()->info("Starting basis is primal feasible, using primal simplex algorithm");
        has_solution = solve_primal(data, params);
    }
    else if (dual_feas)
    {
        log()->info("Starting basis is dual feasible, using dual simplex algorithm");
        has_solution = solve_dual(data, params);
    }
    else
    {
        log()->info("Starting basis is primal and dual infeasible, starting phase 1 with dummy objective");

        // Use 2 phase procedure:
        // 1. Change to dummy dual feasible objective (all -1's) and solve using dual simplex
        // 2. Restore original objective and solve using primal simplex.

        auto original_c = data.c;

        auto dummy_c = Mat{original_c.n_rows(), original_c.n_cols(), 0};
        for (const auto& var : data.non_basics)
        {
            dummy_c(var.index, 0) = -1.0;
        }

        update_primal_objective(data, dummy_c);
        assert(is_dual_feas(data));

        has_solution = solve_dual(data, params);
        assert(!has_artificals_in_basis(data));

        if (has_solution)
        {
            log()->info("Restoring objective for phase 2");
            update_primal_objective(data, original_c);
            has_solution = solve_primal(data, params);
        }
        else
        {
            log()->warn("Infeasible");
        }
    }

    std::optional<Solution> solution;

    if (has_solution)
    {
        solution = extract_solution(model, data);

        log()->debug("---------------------------------------");
        log()->info("Objective = {:.2f} ({} iterations)", solution->objective, data.n_iter);
    }

    return solution;
}

} // namespace jsolve
