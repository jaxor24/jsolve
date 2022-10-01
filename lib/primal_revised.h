#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve
{
std::optional<Solution> solve_simplex_revised(const Model& model);
}