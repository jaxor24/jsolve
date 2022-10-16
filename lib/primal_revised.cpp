#include "primal_revised.h"
#include "constraint.h"
#include "simplex_common.h"

#include "variable.h"

#include "solve_error.h"

#include "matrix.h"
#include "tools.h"

#include "logging.h"

#include <algorithm>

namespace jsolve
{
using Mat = Matrix<double>;

namespace
{
struct Parameters
{
    int max_iter{10000}; // Stopping criteria - max simplex iterations
    double EPS1{1e-8};   // Minimum value to consider as exiting var
    double EPS2{1e-12};  // Protection from division by zero
};

struct SolveData
{
    // Everything we need to do iterations of the revised simplex algorithm.

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

std::optional<std::size_t> choose_entering(const Mat& column, double EPS2)
{
    // Choses most negative variable in the column vector.

    std::optional<std::size_t> entering;
    Mat::value_type current_min{-EPS2};

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

std::optional<std::size_t> choose_leaving(const Mat& num, const Mat& denom, double EPS1)
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

double calc_primal_obj(const Mat& c, const Mat& x_basic, const std::vector<VarData>& basics)
{
    double primal_obj{0.0};

    for (std::size_t curr_idx = 0; curr_idx < x_basic.n_rows(); curr_idx++)
    {
        primal_obj += c(basics[curr_idx].index, 0) * x_basic(curr_idx, 0);
    }

    return primal_obj;
}

double calc_dual_infeas(const Mat& c, const Mat& x_basic, const std::vector<VarData>& basics)
{
    double primal_obj{0.0};

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

    log()->debug(A);

    // Create c column vector = [c]
    Mat c{n, 1, 0.0};
    for (const auto& [n_var, pair] : enumerate(model.get_variables()))
    {
        c(n_var, 0) = pair.second->cost();
    }

    if (model.sense() == Model::Sense::MIN)
    {
        // todo - this making -0
        c *= -1;
    }

    log()->debug(c);

    // Create b (RHS) vector
    Mat b{m, 1, 0.0};
    for (const auto& [n_cons, pair] : enumerate(model.get_constraints()))
    {
        assert(pair.second->type() == Constraint::Type::EQUAL);
        b(n_cons, 0) = pair.second->rhs();
    }

    log()->debug(b);

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

    log()->debug(B);
    log()->debug(N);

    log()->debug(x_basic);
    log()->debug(z_non_basic);

    return {A, c, B, N, x_basic, z_non_basic, basics, non_basics};
}

bool solve_primal(SolveData& data, Parameters params)
{
    // Solve using the primal (revised) simplex algorithm.
    // Uses implementation from Linear Programming (Vanderbei, 2014) p92.
    // Returns true if a solution is present.

    Mat& A = data.A;
    // Mat& c = data.c;
    Mat& B = data.B;
    Mat& N = data.N;
    Mat& x_basic = data.x_basic;
    Mat& z_non_basic = data.z_non_basic;
    std::vector<VarData>& basics = data.basics;
    std::vector<VarData>& non_basics = data.non_basics;
    int& iter = data.n_iter;

    for (; iter <= params.max_iter; iter++)
    {
        log_iteration(iter, data);

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
        // dx = inv(B) * N * ej
        // where j = entering index

        auto b_inverse_n = inverse(B) * N;
        auto dx = b_inverse_n.slice({}, {entering.value()});

        log()->debug(dx);

        // 4. Find the leaving variable
        std::optional<std::size_t> leaving = choose_leaving(x_basic, dx, params.EPS1);

        if (!leaving)
        {
            // Unbounded
            return false;
        }

        // 5. Calculate primal step length
        // t = x/dx

        auto t = x_basic(leaving.value(), 0) / dx(leaving.value(), 0);

        // 6. Calculate dz
        // dz = -1 * transpose(inv(B)*N) * ei
        // where i = leaving index

        auto dz = -1.0 * (b_inverse_n.slice({leaving.value()}, {})).make_transpose();
        log()->debug(dz);

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

        log()->debug(B);
        log()->debug(N);
    }

    if (iter >= params.max_iter)
    {
        log()->warn("Iteration limit ({}) reached.", iter);
    }
    else
    {
        log()->info("Optimal solution found");
    }

    return true;
}

bool solve_dual(SolveData& data, Parameters params)
{
    // Solve using the dual (revised) simplex algorithm.
    // Uses implementation from Linear Programming (Vanderbei, 2014) p92.
    // Returns true if a solution is present.

    Mat& A = data.A;
    // Mat& c = data.c;
    Mat& B = data.B;
    Mat& N = data.N;
    Mat& x_basic = data.x_basic;
    Mat& z_non_basic = data.z_non_basic;
    std::vector<VarData>& basics = data.basics;
    std::vector<VarData>& non_basics = data.non_basics;
    int& iter = data.n_iter;

    for (; iter <= params.max_iter; iter++)
    {
        log_iteration(iter, data);

        // 1. Check optimality
        // 2. Find entering variable
        // Pick minimum (and negative) x_basic

        std::optional<std::size_t> entering = choose_entering(x_basic, params.EPS2);

        if (!entering)
        {
            // Optimal
            return true;
        }

        // 3. Calculate dx (BTRAN)
        // dz = -1 * transpose(inv(B)*N) * ei
        // where i = entering index

        auto b_inverse_n = inverse(B) * N;
        auto dz = -1.0 * (b_inverse_n.slice({entering.value()}, {})).make_transpose();

        log()->debug(dz);

        // 4. Find the leaving variable

        std::optional<std::size_t> leaving = choose_leaving(z_non_basic, dz, params.EPS1);

        if (!leaving)
        {
            // Unbounded
            return false;
        }

        // 5. Calculate dual step length
        // s = z/dz

        auto s = z_non_basic(leaving.value(), 0) / dz(leaving.value(), 0);

        // 6. Calculate dx (FTRAN)
        // dx = inv(B) * N * ej
        // where j = leaving index

        auto dx = b_inverse_n.slice({}, {leaving.value()});
        log()->debug(dx);

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

        log()->debug(B);
        log()->debug(N);
    }

    if (iter >= params.max_iter)
    {
        log()->warn("Iteration limit ({}) reached.", iter);
    }
    else
    {
        log()->info("Optimal solution found");
    }

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
        if (!var_data.slack && !var_data.dummy)
        {
            sol.variables[model.get_variable(var_data.index)->name()] = data.x_basic(idx, 0);
        }
        ++idx;
    }

    for (const auto& var_data : data.non_basics)
    {
        if (!var_data.slack && !var_data.dummy)
        {
            sol.variables[model.get_variable(var_data.index)->name()] = 0;
        }
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

} // namespace

std::optional<Solution> solve_simplex_revised(const Model& model)
{
    // Setup
    SolveData data{init_data(model)};
    Parameters params{};

    bool has_solution{false};

    if (is_primal_feas(data))
    {
        log()->info("Starting basis is primal feasible, using primal simplex algorithm");
        has_solution = solve_primal(data, params);

        if (!has_solution)
        {
            log()->warn("Unbounded");
        }
    }
    else if (is_dual_feas(data))
    {
        log()->info("Starting basis is dual feasible, using dual simplex algorithm");
        has_solution = solve_dual(data, params);

        if (!has_solution)
        {
            log()->warn("Unbounded");
        }
    }
    else
    {
        log()->info("Starting basis is primal and dual infeasible, starting phase 1");

        // Change to dummy dual feasible objective (all +1's)
        auto original_c{data.c};
        auto original_z_non_basic{data.z_non_basic};
        data.c = Mat{original_c.n_rows(), original_c.n_cols(), -1};

        for (std::size_t idx{0}; const auto& var : data.non_basics)
        {
            data.z_non_basic(idx, 0) = -1.0 * data.c(var.index, 0);
            idx++;
        }

        // data.z_non_basic = Mat{original_z_non_basic.n_rows(), original_z_non_basic.n_cols(), 1};

        has_solution = solve_dual(data, params);

        if (has_solution)
        {
            log()->info("Restoring objective for phase 2");

            // Restore objective: z_non_basic = transpose(inv(B)*N)*c_b - c_n
            data.c = original_c;

            auto b_inv_n_t = (inverse(data.B) * data.N).make_transpose();

            log()->debug(b_inv_n_t);

            for (std::size_t idx{0}; const auto& var : data.non_basics)
            {
                data.z_non_basic(idx, 0) = -1.0 * data.c(var.index, 0);
                idx++;
            }

            for (std::size_t idx{0}; const auto& var : data.basics)
            {
                data.z_non_basic.update(
                    {}, {}, data.z_non_basic.slice({}, {}) + b_inv_n_t.slice({}, idx) * data.c(var.index, 0)
                );
                idx++;
            }

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
