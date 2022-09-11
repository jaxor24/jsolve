#include "simplex.h"
#include "primal_revised.h"
#include "primal_standard.h"
#include "simplex_common.h"

namespace jsolve
{
std::optional<Solution> solve(Model& model)
{
    pre_process_model(model);
    return solve_primal_standard(model);
    // return solve_primal_revised(model);
}
} // namespace jsolve