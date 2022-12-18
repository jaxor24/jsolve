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
        throw SolveError("Cannot factor non-square matrix");
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

template <typename U>
Matrix<U> backward_subs(const Matrix<U>& A, const Matrix<U>& b)
{
    // Solves Ax = b via backward subsitution, assuming A is upper triangular.

    int m{static_cast<int>(A.n_rows())};
    int n{static_cast<int>(A.n_cols())};

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

    Matrix<U> x{A.n_rows(), 1};

    for (int i{n - 1}; i >= 0; i--)
    {
        U sum{0};
        for (int j{i + 1}; j < n; j++)
        {
            sum += A(i, j) * x(j, 0);
        }
        x(i, 0) = (b(i, 0) - sum) / A(i, i);
    }

    return x;
}

template <typename U>
Matrix<U> forward_subs(const Matrix<U>& A, const Matrix<U>& b)
{
    // Solves Ax = b via forward subsitution, assuming A is lower triangular.

    int m{static_cast<int>(A.n_rows())};
    int n{static_cast<int>(A.n_cols())};

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

    Matrix<U> x{A.n_rows(), 1};

    for (int i{0}; i < m; i++)
    {
        U sum{0};
        for (int j{0}; j < i; j++)
        {
            sum += A(i, j) * x(j, 0);
        }
        x(i, 0) = (b(i, 0) - sum) / A(i, i);
    }

    return x;
}

} // namespace jsolve