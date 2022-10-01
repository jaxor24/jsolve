#include "simplex.h"
#include "primal_revised.h"
#include "primal_standard.h"
#include "simplex_common.h"

namespace jsolve
{

std::optional<Solution> solve(Model& model, alg_type alg)
{
    pre_process_model(model);

    if (alg == alg_type::STANDARD)
    {
        return solve_primal_standard(model);
    }
    else if (alg == alg_type::REVISED)
    {
        return solve_simplex_revised(model);
    }
    else
    {
        throw std::invalid_argument("Unknown algorithm type");
    }
}
} // namespace jsolve
