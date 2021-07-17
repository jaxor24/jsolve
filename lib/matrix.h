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

	// Iterators -------------------------------------------------------------------------------
	matrix_iterator begin();
	matrix_iterator end();

	const_matrix_iterator cbegin() const;
	const_matrix_iterator cend() const;

	auto enumerate_rows();
	auto enumerate_rows() const;

	auto enumerate_cols();
	auto enumerate_cols() const;

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
	Matrix& operator++();
	Matrix operator++(int);
	Matrix& operator--();
	Matrix operator--(int);

	// Binary - Matrix
	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);
	Matrix& operator*=(const Matrix& rhs);
	
	// Binary - Scalar
	Matrix operator+=(double x);
	Matrix operator-=(double x);
	Matrix operator*=(double x);
	Matrix operator/=(double x);

private:
	std::vector<T> m_data; // [row 1 | row 2 | ... | row n]
	std::size_t m_n_rows{ 0 };
	std::size_t m_n_cols{ 0 };
};

#include "matrix_operator.hpp"
#include "matrix_free_function.hpp"

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

template <typename T>
typename Matrix<T>::matrix_iterator Matrix<T>::begin()
{ 
	return { std::begin(m_data), std::end(m_data), n_cols() };
}

template <typename T>
typename Matrix<T>::matrix_iterator Matrix<T>::end()
{ 
	return { std::end(m_data), std::end(m_data), n_cols() };
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::cbegin() const
{
	return { std::begin(m_data), std::end(m_data), n_cols() };
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::cend() const
{ 
	return { std::end(m_data), std::end(m_data), n_cols() };
}

template <typename T>
auto Matrix<T>::enumerate_rows()
{
	return IterableWrapper<RowEnumerator<matrix_iterator>>{ begin(), end() };
}

template <typename T>
auto Matrix<T>::enumerate_rows() const
{
	return IterableWrapper<RowEnumerator<const_matrix_iterator>>{ cbegin(), cend() };
}

template <typename T>
auto Matrix<T>::enumerate_cols()
{
	return IterableWrapper<ColEnumerator<matrix_iterator>>{ begin(), end() };
}

template <typename T>
auto Matrix<T>::enumerate_cols() const
{
	return IterableWrapper<ColEnumerator<const_matrix_iterator>>{ cbegin(), cend() };
}