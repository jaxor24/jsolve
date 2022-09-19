#include "matrix.h"

template <typename U>
void swap_rows(Matrix<U>& input, std::size_t i, std::size_t j)
{
    if (input.n_rows() <= i || input.n_rows() <= j)
    {
        throw MatrixError("Swap rows exceed matrix dimensions");
    }

    for (std::size_t column = 0; column < input.n_cols(); ++column)
    {
        std::swap(input(i, column), input(j, column));
    }
}
