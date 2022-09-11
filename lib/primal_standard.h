#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve::simplex
{
std::optional<Solution> solve_primal_standard(const Model& model);
}