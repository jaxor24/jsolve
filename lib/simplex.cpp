#include "simplex.h"
#include "primal_standard.h"
#include "primal_revised.h"

namespace jsolve
{
std::optional<Solution> solve(const Model& model)
{
    return solve_primal_standard(model);
}
}