#pragma once

#include "model.h"

#include "matrix.h"
#include "tools.h"

#include <map>

namespace jsolve
{
using Mat = Matrix<double>;

struct VarData
{
    int index{0};       // The index with respect to the original user model
    int subscript{0};   // The subscript to use within the simplex algorithm
    bool slack{false};  // Potentially not needed
    bool dummy{false};

    bool operator==(const VarData&) const = default;
};

struct Locations
{
    std::map<std::size_t, VarData> basics;
    std::map<std::size_t, VarData> non_basics;
};

void pre_process_model(jsolve::Model& model);

} // namespace jsolve
