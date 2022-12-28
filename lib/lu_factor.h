#pragma once

#include "matrix.h"
#include "solve_error.h"

#include <utility>

namespace jsolve
{

template <typename T>
struct lu_result
{
    Matrix<T> L;
    Matrix<T> U;
    std::vector<std::size_t> perm;
};

template <typename T>
lu_result<T> lu_factor(Matrix<T> A)
{
    // LU factorisation of A using the Doolittle method with max magnitude partial pivoting.
    // Returns L, U and P such that LU = PA.
    // TODO:
    // - discard P as it is only needed for tests
    // - avoid explicitly swapping rows of A and work via perm instead

    auto m{A.n_rows()};
    auto n{A.n_cols()};

    if (m != n)
    {
        throw SolveError("Cannot factor non-square matrix");
    }

    lu_result<T> result{.L{n, n}, .U{n, n}, .perm{std::vector<std::size_t>(n, 0)}};

    // Permutation vector
    auto& perm = result.perm;
    std::iota(std::begin(perm), std::end(perm), 0);

    std::size_t imax{0};
    T tol{1e-9};
    T maxA{0.0};
    T absA{0.0};

    for (std::size_t i{0}; i < n; i++)
    {
        // Pivoting
        maxA = 0.0;
        imax = i;

        for (std::size_t k{i}; k < n; k++)
        {
            absA = std::abs(A(k, i));

            if (absA > maxA)
            {
                maxA = absA;
                imax = k;
            }
        }

        if (maxA < tol)
        {
            throw SolveError("LU factor failed, matrix is degenerate");
        }

        if (imax != i)
        {
            std::swap(perm[i], perm[imax]);
            swap_rows(A, i, imax);
        }

        // Factorisation
        for (std::size_t j{i + 1}; j < n; j++)
        {
            A(j, i) = A(j, i) / A(i, i);
            for (std::size_t k{i + 1}; k < n; k++)
            {
                A(j, k) = A(j, k) - (A(j, i) * A(i, k));
            }
        }
    }

    // Extract L, U
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row == col)
            {
                result.U(row, col) = A(row, col);
                result.L(row, col) = 1;
            }
            else if (row > col)
            {
                result.L(row, col) = A(row, col);
            }
            else
            {
                result.U(row, col) = A(row, col);
            }
        }
    }

    log()->trace(A);
    log()->trace(result.L);
    log()->trace(result.U);
    return result;
}

template <typename U>
Matrix<U> backward_subs(const Matrix<U>& A, const Matrix<U>& b, const std::vector<std::size_t>& perm)
{
    // Solves Ax = b via backward substitution including column permutations.
    // A is assumed to be upper triangular, with col swaps given by perm.
    // b is a column who's rows are in the original (non-permuted) order.

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

    if (b.n_rows() != perm.size())
    {
        throw SolveError("Invalid permutation vector size");
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

    // Re-order the solution according to the column permutations
    // TODO: This is bad - we need to copy x to do this!
    auto x_original{x};

    for (std::size_t i{0}; i < n; i++)
    {
        x(perm[i], 0) = x_original(i, 0);
    }

    return x;
}

template <typename U>
Matrix<U> backward_subs(const Matrix<U>& A, const Matrix<U>& b)
{
    auto perm = std::vector<std::size_t>(b.n_rows(), 0);
    std::iota(std::begin(perm), std::end(perm), 0); // [0, 1, ..., n];
    return backward_subs(A, b, perm);
}

template <typename U>
Matrix<U> forward_subs(const Matrix<U>& A, const Matrix<U>& b, const std::vector<std::size_t>& perm)
{
    // Solves Ax = b by forward substitution including row permutations.
    // A is assumed to be lower triangular, with row swaps given by perm.
    // b is a column who's rows are in the original (non-permuted) order.

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

    if (b.n_rows() != perm.size())
    {
        throw SolveError("Invalid permutation vector size");
    }

    Matrix<U> x{A.n_rows(), 1};

    for (int i{0}; i < m; i++)
    {
        U sum{0};
        for (int j{0}; j < i; j++)
        {
            sum += A(i, j) * x(j, 0);
        }
        x(i, 0) = (b(perm[i], 0) - sum) / A(i, i);
    }

    return x;
}

template <typename U>
Matrix<U> forward_subs(const Matrix<U>& A, const Matrix<U>& b)
{
    auto perm = std::vector<std::size_t>(b.n_rows(), 0);
    std::iota(std::begin(perm), std::end(perm), 0); // [0, 1, ..., n];
    return forward_subs(A, b, perm);
}

} // namespace jsolve