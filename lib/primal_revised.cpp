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

std::optional<Solution> solve_primal_revised(const Model& model)
{
    auto num_user_vars = model.get_variables().size();
    auto num_constr = model.get_constraints().size();

    auto n = num_user_vars;
    auto m = num_constr;
    auto total_vars = n + m;

    // Create A with subtrices A = [N | B]
    Locations locations;

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
    for (auto index = 0; index < total_vars; index++)
    {
        if (index < num_user_vars)
        {
            locations.non_basics[index] = {
                index, // These are the only index variables we care about
                index, false, false};
        }
        else
        {
            locations.basics[index] = {index, index, true, false};
        }
    }

    assert(locations.basics.size() + locations.non_basics.size() == total_vars);

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
    auto y_non_basic = -c.slice({0, n - 1}, {});

    log()->info(x_basic);
    log()->info(y_non_basic);

    // ----------------------------------------------------------------
    // Main Loop (Primal)

    return {};
}
} // namespace jsolve