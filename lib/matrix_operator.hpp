#include "matrix.h"

#include <execution>

// Member functions ---------------------------------------------------------------
// Access

template <typename T>
T& Matrix<T>::operator()(const std::size_t row, const std::size_t col)
{
	return m_data.at(row * n_cols() + col);
}

template <typename T>
T Matrix<T>::operator()(const std::size_t row, const std::size_t col) const
{
	return m_data.at(row * n_cols() + col);
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

	Matrix result{ n_rows(), rhs.n_cols(), 0.0 };

	auto func = [this, &result, &rhs](auto lhs_row)
	{
		for (std::size_t lhs_col = 0; lhs_col < n_cols(); lhs_col++)
		{
			for (std::size_t rhs_col = 0; rhs_col < rhs.n_cols(); rhs_col++)
			{
				result(lhs_row, rhs_col) += operator()(lhs_row, lhs_col) * rhs(lhs_col, rhs_col);
			}
		}
	};

	std::vector<std::size_t> lhs_rows(n_rows());
	std::iota(lhs_rows.begin(), lhs_rows.end(), 0);

	std::for_each(std::execution::par, lhs_rows.begin(), lhs_rows.end(), func);

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
	if (approx_equal(x, 0.0))
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
