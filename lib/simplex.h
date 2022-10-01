#pragma once

#include "model.h"
#include "solution.h"

#include <optional>

namespace jsolve
{
enum class alg_type
{
    STANDARD,
    REVISED
};

std::optional<Solution> solve(Model& model, alg_type alg);
} // namespace jsolve
