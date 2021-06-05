#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve::simplex
{
	std::optional<Solution> primal_solve(const Model& model);
}