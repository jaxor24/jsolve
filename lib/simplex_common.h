#pragma once

#include "model.h"

#include "matrix.h"
#include "tools.h"

#include <map>

namespace jsolve
{
using Number = double;
using Mat = Matrix<Number>;

struct VarData
{
    int index{0};      // The index with respect to the original user model
    int subscript{0};  // The subscript to use within the simplex algorithm
    bool slack{false}; // Potentially not needed
    bool dummy{false};
};

struct Locations
{
    std::map<std::size_t, VarData> basics;
    std::map<std::size_t, VarData> non_basics;
};

void pre_process_model(jsolve::Model& model);

} // namespace jsolve
