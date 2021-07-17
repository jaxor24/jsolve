#include "matrix.h"

// Free functions - helpers
template <typename U>
Matrix<U> mult_elem(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	// Element-wise multiplication

	if ((lhs.n_cols() != rhs.n_cols()) || (lhs.n_rows() != rhs.n_rows()))
	{
		throw MatrixError("Cannot element-wise multiply matrices with different dimensions");
	}

	Matrix<U> result = lhs;

	for (std::size_t n_row = 0; n_row < lhs.n_rows(); n_row++)
	{
		for (std::size_t n_col = 0; n_col < lhs.n_cols(); n_col++)
		{
			result(n_row, n_col) *= rhs(n_row, n_col);
		}
	}

	return result;
}

template <typename U>
Matrix<U> div_elem(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	// Element-wise division

	if ((lhs.n_cols() != rhs.n_cols()) || (lhs.n_rows() != rhs.n_rows()))
	{
		throw MatrixError("Cannot element-wise divide matrices with different dimensions");
	}

	Matrix<U> result = lhs;

	for (std::size_t n_row = 0; n_row < lhs.n_rows(); n_row++)
	{
		for (std::size_t n_col = 0; n_col < lhs.n_cols(); n_col++)
		{
			if (approx_equal(rhs(n_row, n_col), 0.0))
			{
				throw MatrixError("Cannot element-wise divide by matrix with zero element");
			}
			else
			{
				result(n_row, n_col) /= rhs(n_row, n_col);
			}
		}
	}

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

	os << "]" << '\n';
	os << '\n';

	return os;
}
