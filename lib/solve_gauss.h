#pragma once

#include "matrix.h"
#include "solve_error.h"

namespace jsolve
{

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
Matrix<U> solve_gauss(const Matrix<U>& A, const Matrix<U>& b)
{
    // Solves the system Ax = b using Gaussian elimination.

    auto m{A.n_rows()};
    auto n{A.n_cols()};

    if (m != n)
    {
        throw SolveError("Cannot solve non-square system");
    }

    if (m != b.n_rows())
    {
        throw SolveError("Inputs A and b must have the same number of rows");
    }

    if (b.n_cols() != 1)
    {
        throw SolveError("Input b must have one column");
    }

    // Form augmented matrix
    Matrix<U> aug{m, n + 1};
    aug.update({}, {0, n - 1}, A);
    aug.update({}, {n, n}, b);

    // Forward elimination
    if (n >= 2)
    {
        for (std::size_t k{0}; k <= n - 2; k++)
        {
            // Partial pivoting
            auto [value, index] = aug.slice({k, n - 1}, {k}).abs().col_max();
            auto ipr = index(0, 0) + k;
            if (ipr != k)
            {
                swap_rows(aug, k, ipr);
            }

            // Elimination
            for (std::size_t i{k + 1}; i <= n - 1; i++)
            {
                auto factor = aug(i, k) / aug(k, k);
                aug.update({i}, {k, n}, aug.slice({i}, {k, n}) - factor * aug.slice({k}, {k, n}));
            }
        }
    }

    // Back substitution
    Matrix<U> x{m, 1};
    x(n - 1, 0) = aug(n - 1, n) / aug(n - 1, n - 1); // Solve the last row

    if (n >= 2)
    {
        for (std::size_t i{n - 2}; i >= 0 && i < x.n_rows(); i--)
        {
            auto temp1 = (aug.slice(i, {i + 1, n - 1}) * x.slice({i + 1, n - 1}, {0}))(0, 0);
            x(i, 0) = (aug(i, n) - temp1) / aug(i, i);
        }
    }

    return x;
}
} // namespace jsolve