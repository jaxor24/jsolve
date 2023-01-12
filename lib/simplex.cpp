#include "simplex.h"
#include "primal_revised.h"
#include "simplex_common.h"

namespace jsolve
{
std::optional<Solution> solve(Model& model)
{
    Timer timer{info_logger(), "Solving"};

    pre_process_model(model);
    return solve_simplex_revised(model);
}
} // namespace jsolve
