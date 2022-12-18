#pragma once

#include "matrix.h"
#include "solve_error.h"

#include <utility>

namespace jsolve
{
template <typename T>
std::pair<Matrix<T>, Matrix<T>> lu_refactor(const Matrix<T>& A)
{
    // Computes the LU factorisation of A using the Doolittle method.

    auto m{A.n_rows()};
    auto n{A.n_cols()};

    if (m != n)
    {
        throw SolveError("Cannot solve non-square system");
    }

    // Explicit L and U for the moment (pack into A soon)
    Matrix<T> L{n, n};
    Matrix<T> U{n, n};

    for (std::size_t i{0}; i < n; i++)
    {
        // Upper, U
        for (std::size_t k{i}; k < n; k++)
        {
            T sum{0};

            for (std::size_t j{0}; j < i; j++)
            {
                sum += L(i, j) * U(j, k);
            }

            U(i, k) = A(i, k) - sum;
        }

        // Lower, L
        for (std::size_t k{i}; k < n; k++)
        {
            if (i == k)
            {
                L(i, k) = 1;
            }
            else
            {
                T sum{0};

                for (std::size_t j{0}; j < i; j++)
                {
                    sum += L(k, j) * U(j, i);
                }

                L(k, i) = (A(k, i) - sum) / U(i, i);
            }
        }
    }

    return {L, U};
}
} // namespace jsolve