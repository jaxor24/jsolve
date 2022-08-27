#include "matrix.h"

// Free functions - helpers
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
    os << "[";
    for (const auto& [n_elem, elem] : enumerate(m.m_data))
    {
        os << elem;

        if (n_elem + 1 != m.m_data.size())
        {
            if ((n_elem + 1) % m.n_cols() == 0)
            {
                os << ";";
            }
            else
            {
                os << ",";
            }
        }
    }

    os << "]";

    return os;
}
