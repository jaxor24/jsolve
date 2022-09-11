#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve::simplex
{
	std::optional<Solution> solve(const Model& model);
}