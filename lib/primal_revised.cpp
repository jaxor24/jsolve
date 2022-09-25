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

std::optional<std::size_t> choose_entering(const Mat& y_non_basic, double EPS2)
{
    std::optional<std::size_t> entering;
    Mat::value_type current_min{-EPS2};

    for (std::size_t curr_idx = 0; curr_idx < y_non_basic.n_rows(); curr_idx++)
    {
        if (y_non_basic(curr_idx, 0) < current_min)
        {
            entering = curr_idx;
            current_min = y_non_basic(curr_idx, 0);
        }
    }

    return entering;
}

std::optional<std::size_t> choose_leaving(const Mat& x_basic, const Mat& dx, double EPS1)
{
    std::optional<std::size_t> leaving;
    Mat::value_type min_ratio{std::numeric_limits<Mat::value_type>::max()};

    for (std::size_t curr_idx = 0; curr_idx < x_basic.n_rows(); curr_idx++)
    {
        if (dx(curr_idx, 0) > EPS1)
        {
            auto curr_ratio{x_basic(curr_idx, 0) / dx(curr_idx, 0)};

            if (!min_ratio)
            {
                min_ratio = curr_ratio;
            }
            else if (curr_ratio < min_ratio)
            {
                min_ratio = curr_ratio;
                leaving = curr_idx;
            }
        }
    }

    return leaving;
}

double primal_obj(const Mat& c, const Mat& x_basic, const std::vector<VarData>& basics)
{
    double primal_obj{0.0};

    for (std::size_t curr_idx = 0; curr_idx < x_basic.n_rows(); curr_idx++)
    {
        primal_obj += c(basics[curr_idx].index, 0) * x_basic(curr_idx, 0);
    }

    return primal_obj;
}

void log_iteration(int iter, double obj_phase_1, double obj_phase_2, bool in_phase_1)
{
    int log_every{1};

    std::string progress{fmt::format(
        "It {:8} Obj {:14.6f} {}", iter, obj_phase_2, in_phase_1 ? fmt::format("P1 Obj {:14.6f}", obj_phase_1) : ""
    )};

    if (iter % log_every == 0)
    {
        log()->info(progress);
    }
    else
    {
        log()->debug(progress);
    }
}
} // namespace

std::optional<Solution> solve_primal_revised(const Model& model)
{
    auto num_user_vars = model.get_variables().size();
    auto num_constr = model.get_constraints().size();

    auto n = num_user_vars;
    auto m = num_constr;
    auto total_vars = n + m;

    Mat A{m, total_vars, 0.0};

    for (const auto& [n_cons, cons_pair] : enumerate(model.get_constraints()))
    {
        // Build B as the identity
        A(n_cons, n + n_cons) = 1.0;

        // Build N using entries from constraints
        auto scale = cons_pair.second->type() == Constraint::Type::GREAT ? -1.0 : 1.0;
        for (const auto& [n_var, var_pair] : enumerate(model.get_variables()))
        {
            auto found_entry = cons_pair.second->get_entries().find(var_pair.second.get());
            if (found_entry != std::end(cons_pair.second->get_entries()))
            {
                A(n_cons, n_var) = scale * found_entry->second;
            }
        }
    }

    // Setup the variable location data
    std::vector<VarData> basics;
    basics.reserve(n);
    std::vector<VarData> non_basics;
    non_basics.reserve(m);

    for (int index = 0; index < total_vars; index++)
    {
        if (index < n)
        {
            non_basics.push_back(
                {index, // These are the only index variables we care about
                 index, false, false}
            );
        }
        else
        {
            basics.push_back({index, index, true, false});
        }
    }

    log()->info(A);

    // Create c column vector = [c | 0]
    Mat c{total_vars, 1, 0.0};
    for (const auto& [n_var, pair] : enumerate(model.get_variables()))
    {
        c(n_var, 0) = pair.second->cost();
    }

    if (model.sense() == Model::Sense::MIN)
    {
        c *= -1;
    }

    log()->info(c);

    // Create b (RHS) vector
    Mat b{m, 1, 0.0};
    for (const auto& [n_cons, pair] : enumerate(model.get_constraints()))
    {
        if (pair.second->type() == Constraint::Type::GREAT)
        {
            // 7x + 2y >= 5 becomes -7x - 2y <= -5
            b(n_cons, 0) = -1 * pair.second->rhs();
        }
        else if (pair.second->type() == Constraint::Type::LESS)
        {
            b(n_cons, 0) = pair.second->rhs();
        }
        else
        {
            // Impossible by construction of vector being iterated.
        }
    }

    log()->info(b);

    // ----------------------------------------------------------------
    // Simplex Stuff

    // For the simplex we need:
    auto B = A.slice({}, {n, n + m - 1});
    auto N = A.slice({}, {0, n - 1});

    log()->info(B);
    log()->info(N);

    auto x_basic = b;
    auto z_non_basic = -c.slice({0, n - 1}, {});

    log()->info(x_basic);
    log()->info(z_non_basic);

    // Tolerances
    double EPS1{1e-8};  // Minimum value to consider as exiting var
    double EPS2{1e-12}; // Protection from division by zero

    if (x_basic.max() < -EPS2)
    {
        throw SolveError("Infeasible intial basis");
    }

    // ----------------------------------------------------------------
    // Main Loop (Primal)
    // Steps follow Figure 6.1 from Vanderbei, p92.

    int max_iter{10000};
    int iter{0};

    for (; iter <= max_iter; iter++)
    {
        log_iteration(iter, 0, primal_obj(c, x_basic, basics), false);

        // 1. Check optimality
        // 2. Find entering variable
        // Pick minimum (and negative) z_non_basic
        std::optional<std::size_t> entering = choose_entering(z_non_basic, EPS2);

        if (!entering)
        {
            log()->info("Optimal solution found: {}", primal_obj(c, x_basic, basics));
            break;
        }

        // 3. Calculate dx
        // dx = inv(B) * N * ej
        // where j = entering index

        auto b_inverse_n = inverse(B) * N;
        auto dx = b_inverse_n.slice({}, {entering.value()});

        log()->debug(dx);

        // 4. Find the leaving variable
        // 5. Calculate primal step length
        // t = x/dx

        std::optional<std::size_t> leaving = choose_leaving(x_basic, dx, EPS1);

        if (!leaving)
        {
            log()->info("Unbounded");
            break;
        }

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
        z_non_basic = z_non_basic - s * dz;

        // 9. Update variables

        B.update({}, {leaving.value()}, A.slice({}, static_cast<std::size_t>(non_basics[entering.value()].index)));
        N.update({}, {entering.value()}, A.slice({}, static_cast<std::size_t>(basics[leaving.value()].index)));
        x_basic(leaving.value(), 0) = t;
        z_non_basic(entering.value(), 0) = s;

        log()->debug(B);
        log()->debug(N);
        std::swap(basics[leaving.value()], non_basics[entering.value()]);
    }

    auto primal = primal_obj(c, x_basic, basics);

    Solution sol{};

    sol.objective = model.sense() == Model::Sense::MIN ? -1.0 * primal : primal;

    for (std::size_t curr_idx = 0; curr_idx < x_basic.n_rows(); curr_idx++)
    {
        if (!basics[curr_idx].slack && !basics[curr_idx].dummy)
        {
            sol.variables[model.get_variable(basics[curr_idx].index)->name()] = x_basic(curr_idx, 0);
        }
    }

    for (std::size_t curr_idx = 0; curr_idx < z_non_basic.n_rows(); curr_idx++)
    {
        if (!non_basics[curr_idx].slack && !non_basics[curr_idx].dummy)
        {
            sol.variables[model.get_variable(non_basics[curr_idx].index)->name()] = z_non_basic(curr_idx, 0);
        }
    }

    if (iter >= max_iter)
    {
        log()->warn("Iteration limit ({}) reached.", iter);
    }
    else
    {
        log()->info("Optimal solution found");
    }

    log()->debug("---------------------------------------");
    log()->info("Objective = {:.2f} ({} iterations)", sol.objective, iter);

    return sol;
}
} // namespace jsolve