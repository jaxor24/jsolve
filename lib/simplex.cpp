#include "simplex.h"
#include "primal_standard.h"

namespace jsolve::simplex
{
std::optional<Solution> solve(const Model& model)
{
    return solve_primal_standard(model);
}
}