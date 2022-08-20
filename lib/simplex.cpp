#include "simplex.h"

#include "constraint.h"
#include "variable.h"

#include "solve_error.h"

#include "matrix.h"
#include "tools.h"

#include "logging.h"

#include <algorithm>

namespace jsolve
{
using Number = double;
using Mat = Matrix<Number>;

struct Var
{
    int index{0};
    int subscript{0};
    bool slack{false};
    bool dummy{false};
};

struct Locations
{
    std::map<std::size_t, Var> basics;
    std::map<std::size_t, Var> non_basics;
};

struct StandardFormModel
{
    Mat c_phase_1;
    Mat c_phase_2;
    Mat A;
    Mat b;
    Locations locations;
};

struct MatrixModel
{
    Mat c;
    Mat A;
    Mat b;
};

void verify_matrix_model(MatrixModel model)
{
    if (model.A.n_rows() != model.b.n_rows())
    {
        throw SolveError("matrix model A row count and b row count different");
    }

    if (model.A.n_cols() != model.c.n_rows())
    {
        throw SolveError("matrix model A col count and b row count different");
    }
}

MatrixModel to_matrix_form(const Model& user_model)
{
    // Convert a raw model into matrix form.

    auto n_user_vars = user_model.get_variables().size();
    auto n_slack_vars = user_model.get_constraints().size();
    auto n_total_vars = n_user_vars + n_slack_vars;

    // Shield against equal constraints (need 2 slacks)
    std::for_each(
        std::begin(user_model.get_constraints()), std::end(user_model.get_constraints()),
        [](const auto& pair) {
            if (pair.second->type() == Constraint::Type::EQUAL)
            {
                throw SolveError("EQUAL constraints unsupported");
            }
        }
    );

    // Objective vector
    Mat objective{n_total_vars, 1, 0.0};

    for (const auto& [n_var, pair] : enumerate(user_model.get_variables()))
    {
        objective(n_var, 0) = pair.second->cost();
    }

    if (user_model.sense() == Model::Sense::MIN)
    {
        objective *= -1;
    }

    log()->info(objective);

    // b (RHS) vector
    Mat rhs{n_slack_vars, 1, 0.0};
    for (const auto& [n_cons, pair] : enumerate(user_model.get_constraints()))
    {
        if (pair.second->type() == Constraint::Type::GREAT)
        {
            // 7x + 2y >= 5 becomes -7x - 2y <= -5
            rhs(n_cons, 0) = -1 * pair.second->rhs();
        }
        else
        {
            rhs(n_cons, 0) = pair.second->rhs();
        }
    }

    log()->info(rhs);

    // A matrix
    Mat a{n_slack_vars, n_total_vars, 0.0};

    for (const auto& [n_cons, cons_pair] : enumerate(user_model.get_constraints()))
    {
        auto scale = cons_pair.second->type() == Constraint::Type::GREAT ? -1.0 : 1.0;

        // Add original constraint entries
        for (const auto& [n_var, var_pair] : enumerate(user_model.get_variables()))
        {
            auto found_entry = cons_pair.second->get_entries().find(var_pair.second.get());

            if (found_entry != std::end(cons_pair.second->get_entries()))
            {
                a(n_cons, n_var) = scale * found_entry->second;
            }
        }

        // Add slack var coefficient
        a(n_cons, n_user_vars + n_cons) = 1;
    }

    log()->info(a);
    return {objective, a, rhs};
}

} // namespace jsolve

namespace jsolve::simplex
{

Locations init_locations(const Mat& A)
{
    Locations locations;

    // Initially, rows of A are the slack vars
    for (auto i = 0; i < static_cast<int>(A.n_rows()); i++)
    {
        locations.basics[i] = {i, i + static_cast<int>(A.n_cols()) - 1, true, false};
    }

    // Initially, cols of A are the user vars
    for (auto i = 0; i < static_cast<int>(A.n_cols()); i++)
    {
        locations.non_basics[i] = {
            i, // These are the only index variables we care about
            i, false, false};
    }

    // Assume phase 1 dummy
    if (true)
    {
        int idx = static_cast<int>(A.n_cols()) - 1;
        locations.non_basics[idx] = {idx, -1, false, true};
    }

    return locations;
}

StandardFormModel to_standard_form(const Model& user_model)
{
    // Returns A, b and c such that:
    // - objective is maximisation
    // - constaints are LHS <= RHS
    // - vars are non-negative
    // - a dummy variable is included for a 2 phase algorithm
    // With:
    // A = n_cons * n_vars (+ 1 dummy)
    // b = n_cons * 1,
    // c = 1 * n_vars (+ 1 dummy)

    // Create a new constraint vector with the equality constraints converted to LESS and GREAT pairs.
    std::vector<Constraint> converted_constraints;
    for (const auto& [n_cons, pair] : enumerate(user_model.get_constraints()))
    {
        if (pair.second->type() == Constraint::Type::EQUAL)
        {
            converted_constraints.emplace_back(*pair.second.get());
            converted_constraints.back().type() = Constraint::Type::LESS;

            converted_constraints.emplace_back(*pair.second.get());
            converted_constraints.back().type() = Constraint::Type::GREAT;
        }
        else
        {
            converted_constraints.emplace_back(*pair.second.get());
        }
    }

    auto num_vars = user_model.get_variables().size() + 1;
    auto num_constr = converted_constraints.size();

    // Phase 1 objective = max [ 0 ... 0 | -1 ]
    Mat objective_phase_1{1, num_vars, 0.0};
    objective_phase_1(0, num_vars - 1) = -1;

    // Phase 2 objective max [ c | 0 ]
    Mat objective_phase_2{1, num_vars, 0.0};
    for (const auto& [n_var, pair] : enumerate(user_model.get_variables()))
    {
        objective_phase_2(0, n_var) = pair.second->cost();
    }

    if (user_model.sense() == Model::Sense::MIN)
    {
        objective_phase_2 *= -1;
    }

    // b (RHS) vector
    Mat rhs{num_constr, 1, 0.0};
    for (const auto& [n_cons, cons] : enumerate(converted_constraints))
    {
        if (cons.type() == Constraint::Type::GREAT)
        {
            // 7x + 2y >= 5 becomes -7x - 2y <= -5
            rhs(n_cons, 0) = -1 * cons.rhs();
        }
        else if (cons.type() == Constraint::Type::LESS)
        {
            rhs(n_cons, 0) = cons.rhs();
        }
        else
        {
            // Impossible by construction of vector being iterated.
        }
    }

    // Augment A matrix with dummy variable = [ A | -1 ]
    Mat A{num_constr, num_vars, 0.0};

    // Add constraint entries
    for (const auto& [n_cons, cons] : enumerate(converted_constraints))
    {
        auto scale = cons.type() == Constraint::Type::GREAT ? -1.0 : 1.0;
        for (const auto& [n_var, pair] : enumerate(user_model.get_variables()))
        {
            auto found_entry = cons.get_entries().find(pair.second.get());
            if (found_entry != std::end(cons.get_entries()))
            {
                A(n_cons, n_var) = scale * found_entry->second;
            }
        }
    }

    // Set the last column to be 1 for the dummy variable
    A.update({}, {num_vars - 1}, Mat{num_constr, 1, -1.0});

    return {objective_phase_1, objective_phase_2, A, rhs, init_locations(A)};
}

void pivot(
    std::size_t pivot_row, std::size_t pivot_col, double& obj_phase_1, double& obj_phase_2, StandardFormModel& model
)
{
    // We pivot on both phase 1 and phase 2 objectives.

    // Extract rows and cols from A
    auto Acol = model.A.slice({}, {pivot_col});
    auto Arow = model.A.slice({pivot_row}, {});
    auto a = model.A(pivot_row, pivot_col);

    // Pivot A
    model.A -= ((Acol * Arow) / a);
    model.A.update(pivot_row, {}, -1 * Arow / a);
    model.A.update({}, pivot_col, +1 * Acol / a);
    model.A(pivot_row, pivot_col) = 1 / a;

    // Pivot b
    auto brow = model.b(pivot_row, 0);
    model.b = model.b - brow * Acol / a;
    model.b(pivot_row, 0) = -1.0 * brow / a;

    // Pivot phase 1 objective
    {
        auto ccol = model.c_phase_1(0, pivot_col);
        auto s = ccol * Arow / a;
        model.c_phase_1 = model.c_phase_1 - ccol * Arow / a;
        model.c_phase_1(0, pivot_col) = ccol / a;
        obj_phase_1 = obj_phase_1 - ccol * brow / a;
    }

    // Pivot phase 2 objective
    {
        auto ccol = model.c_phase_2(0, pivot_col);
        auto s = ccol * Arow / a;
        model.c_phase_2 = model.c_phase_2 - ccol * Arow / a;
        model.c_phase_2(0, pivot_col) = ccol / a;
        obj_phase_2 = obj_phase_2 - ccol * brow / a;
    }

    // Update var location
    auto tmp = model.locations.basics[pivot_row];
    auto tmp1 = model.locations.non_basics[pivot_col];

    model.locations.basics[pivot_row] = tmp1;
    model.locations.non_basics[pivot_col] = tmp;
}

std::optional<std::size_t> get_entering_variable(const Mat& c, bool in_phase_1, Locations& locations, double eps)
{
    // Return the index in c of the entering variable.

    if (in_phase_1)
    {
        // In phase 1 we can pivot on any variable in c
        auto [c_max, c_max_index] = c.row_max();
        if (c_max(0, 0) > eps)
        {
            return c_max_index(0, 0);
        }
        else
        {
            return {};
        }
    }
    else
    {
        std::size_t c_max_index{0};
        Mat::value_type c_max_value{eps};

        for (std::size_t curr_idx = 0; curr_idx < c.n_cols(); curr_idx++)
        {
            if (!locations.non_basics.at(curr_idx).dummy && (c(0, curr_idx) > c_max_value))
            {
                c_max_value = c(0, curr_idx);
                c_max_index = curr_idx;
            }
        }

        if (c_max_value > eps)
        {
            return c_max_index;
        }
        else
        {
            return {};
        }
    }
}

std::optional<std::size_t> get_leaving_variable(
    StandardFormModel& model, std::size_t pivot_col, bool in_phase_1, double eps_zero
)
{
    // Select variable to leave the basis using Bland's rule.

    std::optional<std::size_t> row_idx;
    std::optional<double> min_ratio{std::numeric_limits<double>::infinity()};
    std::optional<int> subscript;

    auto Acol = model.A.slice({}, {pivot_col}); // TODO - get rid of this

    for (const auto [n_row, elem] : enumerate(Acol))
    {
        if (!in_phase_1 && model.locations.basics[n_row].dummy)
        {
            // If in phase 2, we don't care about the dummy var
            continue;
        }
        else if (elem < -eps_zero)
        {
            auto curr_ratio = -model.b(n_row, 0) / elem;

            if (approx_equal(curr_ratio, min_ratio.value(), eps_zero))
            {
                // Bland's rule - only use the new row if it has a lower subscript
                if (!subscript || (model.locations.basics[n_row].subscript < subscript.value()))
                {
                    min_ratio = curr_ratio;
                    row_idx = n_row;
                    subscript = model.locations.basics[n_row].subscript;
                }
            }
            else if (curr_ratio < min_ratio)
            {
                min_ratio = curr_ratio;
                row_idx = n_row;
                subscript = model.locations.basics[n_row].subscript;
            }
        }
    }

    if (!row_idx)
    {
        // No suitable exiting variable (unbounded)
        return {};
    }

    log()->debug("Leaving variable:");
    log()->debug("With a min ratio value {} at row {}", min_ratio.value(), row_idx.value());

    return row_idx;
}

void log_iteration(int iter, double obj_phase_1, double obj_phase_2, bool in_phase_1)
{
    int log_every{100};

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

Solution extract_solution(const Model& user_model, const StandardFormModel& model, double obj_phase_2)
{ 
    // Extract solution for each variable

    Solution sol;

    sol.objective = user_model.sense() == Model::Sense::MIN ? -1.0 * obj_phase_2 : obj_phase_2;

    for (auto& [idx, var] : model.locations.basics)
    {
        if (!var.slack && !var.dummy)
        {
            sol.variables[user_model.get_variable(var.index)->name()] = model.b(idx, 0);
        }
    }

    for (auto& [idx, var] : model.locations.non_basics)
    {
        if (!var.slack && !var.dummy)
        {
            sol.variables[user_model.get_variable(var.index)->name()] = 0;
        }
    }

    return sol;
}

std::optional<Solution> primal_solve(const Model& user_model)
{
    // Follows the implementation in Chapter 4 p46. of "Linear Programming" 2014.

    Timer timer{info_logger(), "Primal Simplex Algorithm"};

    int max_iter{10000};     // Stopping critera - max simplex iterations
    double eps_column{1e-5}; // Tolerance for selection of entering variable. i.e. optimality tolerance.
    double eps_zero{1e-8};   // Needs to be the same as that used in matrix/double

    auto model{to_standard_form(user_model)};
    model.A *= -1; // -1 to convert to dictionary with basic vars on LHS

    bool in_phase_1{false};

    // Determine if phase 1 is needed
    auto [b_mins, b_min_indices] = model.b.col_min();
    if (b_mins(0, 0) < 0)
    {
        in_phase_1 = true;
    }

    double obj_phase_1{0.0};
    double obj_phase_2{0.0};
    int iter{0};

    if (in_phase_1)
    {
        // Do first pivot of phase 1 variable and most negative RHS row
        log()->debug("c_phase_1 = {}", model.c_phase_1);
        log()->debug("c_phase_2 = {}", model.c_phase_2);
        log()->debug("A = {}", model.A);
        log()->debug("b = {}", model.b);

        auto [row_min, row_min_idx] = model.b.col_min();
        auto col_idx = model.c_phase_1.n_cols() - 1;

        pivot(row_min_idx(0, 0), col_idx, obj_phase_1, obj_phase_2, model);

        log_iteration(iter, obj_phase_1, obj_phase_2, in_phase_1);
        iter++;
    }

    auto& current_objective = in_phase_1 ? model.c_phase_1 : model.c_phase_2;

    std::optional<std::size_t> entering_idx{};
    std::optional<std::size_t> leaving_idx{};

    for (; iter <= max_iter; iter++)
    {
        log()->debug("c = {}", current_objective);
        log()->debug("A = {}", model.A);
        log()->debug("b = {}", model.b);

        entering_idx = get_entering_variable(current_objective, in_phase_1, model.locations, eps_column);

        if (!entering_idx && in_phase_1)
        {
            if (obj_phase_1 < -eps_column)
            {
                log()->warn("Model is infeasible");
                return {};
            }
            else
            {
                in_phase_1 = false;
                current_objective = model.c_phase_2;
                entering_idx = get_entering_variable(current_objective, in_phase_1, model.locations, eps_column);
            }
        }

        if (!entering_idx)
        {
            // Optimal
            break;
        }

        auto col_idx{entering_idx.value()};

        log()->debug("Entering variable:");
        log()->debug("Objective max coeff {} at col {}", current_objective(0, col_idx), col_idx);

        leaving_idx = get_leaving_variable(model, col_idx, in_phase_1, eps_zero);

        if (!leaving_idx)
        {
            log()->warn("Model is unbounded");
            return {};
        }

        auto row_idx{leaving_idx.value()};

        log()->debug("Pivot on element {} at {},{}", model.A(row_idx, col_idx), row_idx, col_idx);

        pivot(row_idx, col_idx, obj_phase_1, obj_phase_2, model);

        log_iteration(iter, obj_phase_1, obj_phase_2, in_phase_1);
    }

    Solution sol{extract_solution(user_model, model, obj_phase_2)};

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
} // namespace jsolve::simplex
