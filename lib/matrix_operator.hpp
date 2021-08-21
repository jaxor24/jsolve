#include "matrix.h"

// Member functions ---------------------------------------------------------------
// Access

template <typename T>
T& Matrix<T>::operator()(const std::size_t row, const std::size_t col)
{
	if (row >= n_rows() || col >= n_cols())
	{
		throw MatrixError("Cannot access out of bounds");
	}

	return m_data[row * n_cols() + col];
}

template <typename T>
T Matrix<T>::operator()(const std::size_t row, const std::size_t col) const
{
	if (row >= n_rows() || col >= n_cols())
	{
		throw MatrixError("Cannot access out of bounds");
	}

	return m_data[row * n_cols() + col];
}

// Unary
template <typename T>
Matrix<T>& Matrix<T>::operator++()
{
	std::for_each(std::begin(m_data), std::end(m_data), [](auto& elem) { elem++; });
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator++(int)
{
	Matrix tmp(*this);
	operator++();
	return tmp;
}

template <typename T>
Matrix<T>& Matrix<T>::operator--()
{
	std::for_each(std::begin(m_data), std::end(m_data), [](auto& elem) { elem--; });
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator--(int)
{
	Matrix tmp(*this);
	operator--();
	return tmp;
}

// Binary - Matrix
template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
	if (n_rows() != rhs.n_rows())
	{
		throw MatrixError("Cannot add matrices with different row count");
	}

	if (n_cols() != rhs.n_cols())
	{
		throw MatrixError("Cannot add matrices with different col count");
	}

	auto it1 = std::begin(m_data);
	auto it2 = std::begin(rhs.m_data);

	for (; it1 != std::end(m_data) && it2 != std::end(rhs.m_data); ++it1, ++it2)
	{
		*(it1) += *(it2);
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs)
{
	if (n_rows() != rhs.n_rows())
	{
		throw MatrixError("Cannot subtract matrices with different row count");
	}

	if (n_cols() != rhs.n_cols())
	{
		throw MatrixError("Cannot subtract matrices with different col count");
	}

	auto it1 = std::begin(m_data);
	auto it2 = std::begin(rhs.m_data);

	for (; it1 != std::end(m_data) && it2 != std::end(rhs.m_data); ++it1, ++it2)
	{
		*(it1) -= *(it2);
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs)
{
	if (n_cols() != rhs.n_rows())
	{
		throw MatrixError(
			"Cannot multiply dimensions {}x{} and {}x{}",
			n_rows(),
			n_cols(),
			rhs.n_rows(),
			rhs.n_cols()
		);
	}
	// (r1 x c1).(r2 x c2) = r1 x c2
	Matrix result{ n_rows(), rhs.n_cols() };

	for (std::size_t lhs_n_r = 0; lhs_n_r < n_rows(); lhs_n_r++)
	{
		for (std::size_t rhs_n_c = 0; rhs_n_c < rhs.n_cols(); rhs_n_c++)
		{
			// For this LHS row and RHS col, sum product the elements.
			value_type element{ 0.0 };

			for (std::size_t lhs_n_c = 0; lhs_n_c < n_cols(); lhs_n_c++)
			{
				element += operator()(lhs_n_r, lhs_n_c) * rhs(lhs_n_c, rhs_n_c);
			}

			result(lhs_n_r, rhs_n_c) = element;
		}
	}

	m_data = result.m_data;
	m_n_cols = result.m_n_cols;
	m_n_rows = result.m_n_rows;
	return *this;
}

// Binary - Scalar
template <typename T>
Matrix<T> Matrix<T>::operator+=(double x)
{
	std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem += x; });
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-=(double x)
{
	std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem -= x; });
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*=(double x)
{
	std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem *= x; });
	return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator/=(double x)
{
	if (approx_equal_new(x, 0.0))
	{
		throw MatrixError("Scalar division by zero");
	}

	std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem /= x; });
	return *this;
}

// Free functions -----------------------------------------------------------------
// Binary - Matrix
template <typename U>
Matrix<U> operator+(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs += rhs;
	return lhs;
}

template <typename U>
Matrix<U> operator-(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs -= rhs;
	return lhs;
}

template <typename U>
Matrix<U> operator*(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs *= rhs;
	return lhs;
}

// Binary - Scalar
template <typename U>
Matrix<U> operator+(Matrix<U> lhs, double x)
{
	lhs += x;
	return lhs;
}

template <typename U>
Matrix<U> operator+(double x, Matrix<U> rhs)
{
	rhs += x;
	return rhs;
}

template <typename U>
Matrix<U> operator-(Matrix<U> lhs, double x)
{
	lhs -= x;
	return lhs;
}

template <typename U>
Matrix<U> operator-(double x, Matrix<U> rhs)
{
	rhs -= x;
	return rhs;
}

template <typename U>
Matrix<U> operator*(Matrix<U> lhs, double x)
{
	lhs *= x;
	return lhs;
}

template <typename U>
Matrix<U> operator*(double x, Matrix<U> rhs)
{
	rhs *= x;
	return rhs;
}

template <typename U>
Matrix<U> operator/(Matrix<U> lhs, double x)
{
	lhs /= x;
	return lhs;
}

template <typename U>
Matrix<U> operator/(double x, Matrix<U> rhs)
{
	rhs /= x;
	return rhs;
}

// Comparison
template <typename U>
bool operator==(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	return lhs.m_data == rhs.m_data;
}

template <typename U>
bool operator!=(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	return !operator==(lhs, rhs);
}

template <typename U>
Matrix<U> operator<(const Matrix<U>& lhs, double x)
{
	// Element-wise e < x

	Matrix<U> result{ lhs.n_rows(), lhs.n_cols(), false };

	for (std::size_t n_row = 0; n_row < lhs.n_rows(); n_row++)
	{
		for (std::size_t n_col = 0; n_col < lhs.n_cols(); n_col++)
		{
			result(n_row, n_col) = lhs(n_row, n_col) < x;
		}
	}

	return result;
}

template <typename U>
Matrix<U> operator>(const Matrix<U>& lhs, double x)
{
	// Element-wise e > x

	Matrix<U> result{ lhs.n_rows(), lhs.n_cols(), false };

	for (std::size_t n_row = 0; n_row < lhs.n_rows(); n_row++)
	{
		for (std::size_t n_col = 0; n_col < lhs.n_cols(); n_col++)
		{
			result(n_row, n_col) = lhs(n_row, n_col) > x;
		}
	}

	return result;
}
