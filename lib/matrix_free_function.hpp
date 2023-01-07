#include "matrix.h"
#include <iomanip>

// Free functions - helpers

template <typename U>
void swap_rows(Matrix<U>& input, std::size_t i, std::size_t j)
{
    if (input.n_rows() <= i || input.n_rows() <= j)
    {
        throw MatrixError("Swap rows exceed matrix dimensions");
    }

    for (std::size_t column{0}; column < input.n_cols(); ++column)
    {
        std::swap(input(i, column), input(j, column));
    }
}

template <typename T>
Matrix<T> eye(std::size_t n)
{
    // Constructs an identity matrix.

    Matrix<T> m{n, n, 0};

    for (std::size_t i{0}; i < n; i++)
    {
        m(i, i) = 1;
    }

    return m;
}

template <typename U, typename F = std::multiplies<U>>
Matrix<U> mult_elem(const Matrix<U>& lhs, const Matrix<U>& rhs, F func = F{})
{
    // Element-wise multiplication

    if ((lhs.n_cols() != rhs.n_cols()) || (lhs.n_rows() != rhs.n_rows()))
    {
        throw MatrixError("Cannot element-wise multiply matrices with different dimensions");
    }

    Matrix<U> result = lhs;

    std::transform(lhs.cbegin(), lhs.cend(), rhs.cbegin(), std::begin(result), func);

    return result;
}

template <typename U, typename F = std::divides<U>>
Matrix<U> div_elem(const Matrix<U>& lhs, const Matrix<U>& rhs, F func = F{})
{
    // Element-wise division

    if ((lhs.n_cols() != rhs.n_cols()) || (lhs.n_rows() != rhs.n_rows()))
    {
        throw MatrixError("Cannot element-wise divide matrices with different dimensions");
    }

    Matrix<U> result = lhs;

    std::transform(lhs.cbegin(), lhs.cend(), rhs.cbegin(), std::begin(result), func);

    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
    os << std::fixed << std::setprecision(4);
    os << "\n";
    os << "[";
    for (const auto& [n_elem, elem] : enumerate(m.m_data))
    {
        os << std::setw(7) << elem;

        if (n_elem + 1 != m.m_data.size())
        {
            if ((n_elem + 1) % m.n_cols() == 0)
            {
                os << "\n";
                os << "  ";
            }
            else
            {
                os << ",";
            }
        }
    }

    os << "]\n";

    return os;
}
