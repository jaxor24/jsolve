#pragma once

#include "matrix_error.h"
#include "matrix_range.h"
#include "matrix_iterator.h"

#include "tools.h"
#include "logging.h"

#include <vector>
#include <numeric>
#include <optional>
#include <iostream>

template <typename T>
class Matrix
{
public:
	typedef T value_type;
	typedef AllIterator<T> matrix_iterator;
	typedef AllIterator<const T> const_matrix_iterator;

	typedef RowIterator<T> row_iterator;
	typedef ColIterator<T> col_iterator;

	explicit Matrix(std::size_t r, std::size_t c);
	explicit Matrix(std::size_t r, std::size_t c, T initial);

	std::size_t n_rows() const;
	std::size_t n_cols() const;

	T max() const;
	T min() const;
	T sum() const;

	std::pair<Matrix<T>, Matrix<std::size_t>> row_max() const;
	std::pair<Matrix<T>, Matrix<std::size_t>> row_min() const;

	std::pair<Matrix<T>, Matrix<std::size_t>> col_max() const;
	std::pair<Matrix<T>, Matrix<std::size_t>> col_min() const;

	Matrix make_transpose() const;
	Matrix slice(Range rows, Range cols) const;

	void update(Range rows, Range cols, Matrix values);

	// Iterators
	matrix_iterator begin() { return { std::begin(m_data), std::end(m_data), n_cols() }; }
	matrix_iterator end() { return { std::end(m_data), std::end(m_data), n_cols() }; }

	const_matrix_iterator cbegin() const { return { std::begin(m_data), std::end(m_data), n_cols() }; }
	const_matrix_iterator cend() const { return { std::end(m_data), std::end(m_data), n_cols() }; }

	auto enumerate_rows()
	{
		return IterableWrapper<RowEnumerator<matrix_iterator>>{ begin(), end() };
	}

	auto enumerate_rows() const
	{
		return IterableWrapper<RowEnumerator<const_matrix_iterator>>{ cbegin(), cend() };
	}

	auto enumerate_cols()
	{
		return IterableWrapper<ColEnumerator<matrix_iterator>>{ begin(), end() };
	}

	auto enumerate_cols() const
	{
		return IterableWrapper<ColEnumerator<const_matrix_iterator>>{ cbegin(), cend() };
	}

	// Operators -------------------------------------------------------------------------------
	
	// Access
	T& operator()(const std::size_t row, const std::size_t col);
	T operator()(const std::size_t row, const std::size_t col) const;
	
	// Put-to
	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

	// Comparison
	template <typename U>
	friend bool operator==(const Matrix<U>& lhs, const Matrix<U>& rhs);
	template <typename U>
	friend bool operator!=(const Matrix<U>& lhs, const Matrix<U>& rhs);

	// Unary
	Matrix& operator++()
	{
		std::for_each(std::begin(m_data), std::end(m_data), [](auto& elem) { elem++; });
		return *this;
	}
	Matrix operator++(int)
	{
		Matrix tmp(*this);
		operator++();
		return tmp;
	}
	Matrix& operator--()
	{
		std::for_each(std::begin(m_data), std::end(m_data), [](auto& elem) { elem--; });
		return *this;
	}
	Matrix operator--(int)
	{
		Matrix tmp(*this);
		operator--();
		return tmp;
	}

	// Binary - Matrix
	Matrix& operator+=(const Matrix& rhs)
	{
		if (n_rows() != rhs.n_rows())
		{
			throw MatrixError("Cannot add matrices with different row count");
		}

		if (n_cols() != rhs.n_cols())
		{
			throw MatrixError("Cannot add matrices with different col count");
		}

		for (const auto& [n_row, row] : enumerate(m_data))
		{
			for (const auto& [n_col, elem] : enumerate(row))
			{
				elem += rhs(n_row, n_col);
			}
		}

		return *this;
	}
	Matrix& operator-=(const Matrix& rhs)
	{
		if (n_rows() != rhs.n_rows())
		{
			throw MatrixError("Cannot subtract matrices with different row count");
		}

		if (n_cols() != rhs.n_cols())
		{
			throw MatrixError("Cannot subtract matrices with different col count");
		}

		for (const auto& [n_row, row] : enumerate(m_data))
		{
			for (const auto& [n_col, elem] : enumerate(row))
			{
				elem -= rhs(n_row, n_col);
			}
		}

		return *this;
	}
	Matrix& operator*=(const Matrix& rhs)
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
	Matrix operator+=(double x)
	{
		std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem += x; });
		return *this;
	}
	Matrix operator-=(double x)
	{
		std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem -= x; });
		return *this;
	}
	Matrix operator*=(double x)
	{
		std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem *= x; });
		return *this;
	}
	Matrix operator/=(double x)
	{
		if (approx_equal(x, 0.0))
		{
			throw MatrixError("Scalar division by zero");
		}

		std::for_each(std::begin(m_data), std::end(m_data), [x](auto& elem) { elem /= x; });
		return *this;
	}

private:
	std::vector<T> m_data; // [row 1 | row 2 | ... | row n]
	std::size_t m_n_rows{ 0 };
	std::size_t m_n_cols{ 0 };
};

// Free functions - operators
// Operator+
template <typename U>
Matrix<U> operator+(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs += rhs;
	return lhs;
}

template <typename U>
Matrix<U> operator+(Matrix<U> lhs, double x)
{
	lhs += x;
	return lhs;
}

// Operator-
template <typename U>
Matrix<U> operator-(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs -= rhs;
	return lhs;
}

template <typename U>
Matrix<U> operator-(Matrix<U> lhs, double x)
{
	lhs -= x;
	return lhs;
}

// Operator*
template <typename U>
Matrix<U> operator*(Matrix<U> lhs, const Matrix<U>& rhs)
{
	lhs *= rhs;
	return lhs;
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

// Operator/
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
	for (const auto& [n_elem, elem]: enumerate(m.m_data))
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

// Matrix:: member functions
template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c) 
	: 
	Matrix(r, c, T{ 0 })
{}

template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c, T initial) 
	: 
	m_n_rows{ r }, 
	m_n_cols{ c }
{
	if ((r == 0) || (c == 0))
	{
		throw MatrixError("Cannot construct matrix with zero row/col count");
	}

	if ((r < 0) || (c < 0))
	{
		throw MatrixError("Cannot construct matrix with negative row/col count");
	}

	m_data.resize(r*c, initial);
}

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

template <typename T>
std::size_t Matrix<T>::n_rows() const
{
	return m_n_rows;
}

template <typename T>
std::size_t Matrix<T>::n_cols() const
{
	return m_n_cols;
}

template <typename T>
T Matrix<T>::max() const
{
	return *std::max_element(std::begin(m_data), std::end(m_data));
}

template <typename T>
T Matrix<T>::min() const
{
	return* std::min_element(std::begin(m_data), std::end(m_data));
}

template <typename T>
T Matrix<T>::sum() const
{
	return std::accumulate(std::begin(m_data), std::end(m_data), T{ 0 });
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::row_max() const
{
	Matrix<T> values{ n_rows(), 1, std::numeric_limits<T>::lowest() };
	Matrix<std::size_t> indices{ n_rows(), 1, 0 };

	for (auto [n_row, row] : enumerate_rows())
	{
		auto it = std::max_element(std::begin(row), std::end(row));
		values(n_row, 0) = *it;
		indices(n_row, 0) = std::distance(std::begin(row), it);
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::row_min() const
{
	Matrix<T> values{ n_rows(), 1, std::numeric_limits<T>::max() };
	Matrix<std::size_t> indices{ n_rows(), 1, 0 };

	for (auto [n_row, row] : enumerate_rows())
	{
		auto it = std::min_element(std::begin(row), std::end(row));
		values(n_row, 0) = *it;
		indices(n_row, 0) = std::distance(std::begin(row), it);
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::col_max() const
{
	Matrix<T> values{ 1, n_cols(), std::numeric_limits<T>::lowest() };
	Matrix<std::size_t> indices{ 1, n_cols(), 0 };

	for (auto [n_col, col] : enumerate_cols())
	{
		auto it = std::max_element(std::begin(col), std::end(col));
		values(0, n_col) = *it;
		indices(0, n_col) = std::distance(std::begin(col), it);
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::col_min() const
{
	Matrix<T> values{ 1, n_cols(), std::numeric_limits<T>::max() };
	Matrix<std::size_t> indices{ 1, n_cols(), 0 };

	for (auto [n_col, col] : enumerate_cols())
	{
		auto it = std::min_element(std::begin(col), std::end(col));
		values(0, n_col) = *it;
		indices(0, n_col) = std::distance(std::begin(col), it);
	}

	return { values, indices };
}

template <typename T>
Matrix<T> Matrix<T>::make_transpose() const
{
	Matrix result{ n_cols(), n_rows()};

	for (auto [n_row, row] : enumerate_rows())
	{
		for (auto [n_col, elem] : enumerate(row))
		{
			result(n_col, n_row) = elem;
		}
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::slice(Range rows, Range cols) const
{
	if (!rows && !cols)
	{
		return *this;
	}

	Matrix result{ 
		rows ? rows.size() : n_rows(), 
		cols ? cols.size() : n_cols(),
		0.0
	};

	std::size_t result_row{ 0 };
	for (auto [n_row, curr_row] : enumerate_rows())
	{
		if (!rows || ((n_row >= rows.start()) && (n_row <= rows.end())))
		{
			std::size_t result_col{ 0 };
			for (const auto& [n_col, elem] : enumerate(curr_row))
			{
				if (!cols || ((n_col >= cols.start()) && (n_col <= cols.end())))
				{
					result(result_row, result_col) = elem;
					result_col++;
				}
			}
			result_row++;
		}
	}

	return result;
}

template <typename T>
void Matrix<T>::update(Range rows, Range cols, Matrix sub)
{
	// Update the specified range with the values in the provided sub matrix.

	// Check inputs
	if (rows)
	{
		// Rows must match sub matrix size
		if (rows.size() != sub.n_rows())
		{
			throw MatrixError("Row range of size is inconsistent with sub matrix", rows.size(), sub.n_rows());
		}
	}
	else
	{
		// Rows must match target matrix size
		if (n_rows() != sub.n_rows())
		{
			throw MatrixError("Cannot update all {} rows with a sub-matrix of {} rows", n_rows(), sub.n_rows());
		}
	}

	if (cols)
	{
		// Cols must match sub matrix size
		if (cols.size() != sub.n_cols())
		{
			throw MatrixError("Col range of size is inconsistent with sub matrix", cols.size(), sub.n_cols());
		}
	}
	else
	{
		// Cols must match target matrix size
		if (n_cols() != sub.n_cols())
		{
			throw MatrixError("Cannot update all {} cols with a sub-matrix of {} cols", n_cols(), sub.n_cols());
		}
	}

	// Shortcut for replacing entire matrix
	if (!rows && !cols)
	{
		*this = sub;
		return;
	}

	// Update matrix with sub matrix
	std::size_t result_row{ 0 };
	for (auto [n_row, row] : enumerate_rows())
	{
		if (!rows || ((n_row >= rows.start()) && (n_row <= rows.end())))
		{
			std::size_t result_col{ 0 };
			for (auto [n_col, elem] : enumerate(row))
			{
				if (!cols || ((n_col >= cols.start()) && (n_col <= cols.end())))
				{
					elem = sub(result_row, result_col);
					result_col++;
				}
			}
			result_row++;
		}
	}
}
