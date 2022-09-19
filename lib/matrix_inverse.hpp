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

template <typename U>
void rref(Matrix<U>& input)
{
    std::size_t rows{input.n_rows()};
    std::size_t columns{input.n_cols()};

    for (std::size_t row = 0, lead = 0; row < rows && lead < columns; ++row, ++lead)
    {
        std::size_t i{row};
        while (input(i, lead) == 0)
        {
            if (++i == rows)
            {
                i = row;
                if (++lead == columns)
                {
                    return;
                }
            }
        }

        swap_rows(input, i, row);

        if (input(row, lead) != 0)
        {
            U factor{input(row, lead)};
            for (std::size_t column = 0; column < columns; ++column)
            {
                input(row, column) /= factor;
            }
        }

        for (std::size_t j = 0; j < rows; ++j)
        {
            if (j == row)
            {
                continue;
            }

            U factor{input(j, lead)};

            for (std::size_t column = 0; column < columns; ++column)
            {
                input(j, column) -= factor * input(row, column);
            }
        }
    }
}

template <typename U>
Matrix<U> inverse(const Matrix<U>& input)
{
    // Calculates the matrix inverse using Gaussian Elimination

    if (input.n_cols() != input.n_rows())
    {
        throw MatrixError("Cannot invert non-square matrix");
    }

    std::size_t rows{input.n_rows()};

    Matrix<U> tmp{rows, 2 * rows, 0};
    for (std::size_t row = 0; row < rows; ++row)
    {
        for (std::size_t column = 0; column < rows; ++column)
        {
            tmp(row, column) = input(row, column);
        }

        tmp(row, row + rows) = 1;
    }

    rref(tmp);

    Matrix<U> inv{rows, rows};
    for (std::size_t row = 0; row < rows; ++row)
    {
        for (std::size_t column = 0; column < rows; ++column)
        {
            inv(row, column) = tmp(row, column + rows);
        }
    }
    return inv;
}
