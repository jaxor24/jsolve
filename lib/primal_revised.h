#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve
{
std::optional<Solution> solve_primal_revised(const Model& model);
}