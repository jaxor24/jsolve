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
lu_result<T> lu_refactor(const Matrix<T>& A)
{
    // LU factorisation of A using the Doolittle method with max magnitude partial pivoting.
    // The row swaps are incorporated into L (so it may not be lower triangular), but LU = A.

    // This uses a permutation vector to track where row swaps in A have occurred, rather than
    // permuting A itself.

    auto m{A.n_rows()};
    auto n{A.n_cols()};

    if (m != n)
    {
        throw SolveError("Cannot factor non-square matrix");
    }

    lu_result<T> result{.L{n, n}, .U{n, n}, .perm{std::vector<std::size_t>(n, 0)}};

    auto& L = result.L;
    auto& U = result.U;
    auto& perm = result.perm;

    // Permutation vector
    std::iota(std::begin(perm), std::end(perm), 0);

    for (std::size_t k{0}; k < n; k++)
    {
        // Partial pivoting
        std::size_t p{k};
        for (std::size_t t{k + 1}; t < n; t++)
        {
            if (std::abs(A(perm[t], k)) > std::abs(A(perm[p], k)))
            {
                p = t;
            }
        }
        if (p > k)
        {
            std::swap(perm[p], perm[k]);
        }

        // Start stuff

        // Set the L diagonal
        L(perm[k], k) = 1;

        // Set the U diagonal
        {
            T s1{0.0};
            for (std::size_t t{0}; t <= k; t++)
            {
                s1 += L(perm[k], t) * U(t, k);
            }
            U(k, k) = (A(perm[k], k) - s1) / L(perm[k], k);
        }

        // Upper
        for (std::size_t j{k + 1}; j < n; j++)
        {
            T sum{0.0};
            for (std::size_t t{0}; t <= k; t++)
            {
                sum += L(perm[k], t) * U(t, j);
            }
            U(k, j) = (A(perm[k], j) - sum) / L(perm[k], k);
        }

        // Lower
        for (std::size_t i{k + 1}; i < n; i++)
        {
            T sum{0.0};
            for (std::size_t t{0}; t <= k; t++)
            {
                sum += L(perm[i], t) * U(t, k);
            }
            L(perm[i], k) = (A(perm[i], k) - sum) / U(k, k);
        }
    }

    return result;
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
Matrix<U> forward_subs(const Matrix<U>& A, const Matrix<U>& b, const std::vector<std::size_t>& perm)
{
    // Solves Ax = b by forward substituion, with the row permuations of A and b described by perm.

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
            sum += A(perm[i], j) * x(j, 0);
        }
        x(i, 0) = (b(perm[i], 0) - sum) / A(perm[i], i);
    }

    return x;
}

template <typename U>
Matrix<U> forward_subs(const Matrix<U>& A, const Matrix<U>& b)
{
    // Solves Ax = b by forward substitution. Assumes A is lower diagonal.
    auto perm = std::vector<std::size_t>(b.n_rows(), 0);
    std::iota(std::begin(perm), std::end(perm), 0); // [0, 1, ..., n];
    return forward_subs(A, b, perm);
}

} // namespace jsolve