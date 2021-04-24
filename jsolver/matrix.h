#pragma once

#include "tools.h"

#include <vector>
#include <iostream>
#include <stdexcept>


template <typename T>
class Matrix
{
public:
	typedef T value_type;

	explicit Matrix(std::size_t r, std::size_t c);
	explicit Matrix(std::size_t r, std::size_t c, T initial);

	std::size_t n_rows() const;
	std::size_t n_cols() const;

	auto get_impl() const;

	Matrix transpose();

	// Operators
	// Access
	double& operator()(const std::size_t row, const std::size_t col);
	double operator()(const std::size_t row, const std::size_t col) const;

	//// Matrix
	template <typename U>
	friend Matrix<U> operator+(const Matrix<U>& lhs, const Matrix<U>& rhs);

	template <typename U>
	friend Matrix<U> operator-(const Matrix<U>& lhs, const Matrix<U>& rhs);
	//Matrix operator*(Matrix&);
	//// Scalar
	//Matrix operator+(double);
	//Matrix operator-(double);
	//Matrix operator*(double);
	//Matrix operator/(double);

private:
	std::size_t m_rows;
	std::size_t m_cols;
	std::vector<std::vector<T>> m_data;
};

class MatrixError : public std::runtime_error
{
public:
	explicit MatrixError(const std::string& msg) : std::runtime_error(msg) {}
	virtual ~MatrixError() throw () {}
};


// Operators
template <typename T>
double& Matrix<T>::operator()(const std::size_t row, const std::size_t col)
{
	return m_data[row][col];
}

template <typename T>
double Matrix<T>::operator()(const std::size_t row, const std::size_t col) const
{
	return m_data[row][col];
}

template <typename U>
Matrix<U> operator+(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	if (lhs.n_rows() != rhs.n_rows())
	{
		throw MatrixError("Cannot add matrices with different row count");
	}

	if (lhs.n_cols() != rhs.n_cols())
	{
		throw MatrixError("Cannot add matrices with different col count");
	}

	Matrix<U> result{ lhs.n_rows(), lhs.n_cols() };

	for (const auto& [n_row, lhs_row] : enumerate(lhs.m_data))
	{
		for (const auto& [n_col, lhs_elem] : enumerate(lhs_row))
		{
			result(n_row, n_col) = lhs_elem + rhs(n_row, n_col);
		}
	}

	return result;
}

template <typename U>
Matrix<U> operator-(const Matrix<U>& lhs, const Matrix<U>& rhs)
{
	if (lhs.n_rows() != rhs.n_rows())
	{
		throw MatrixError("Cannot add matrices with different row count");
	}

	if (lhs.n_cols() != rhs.n_cols())
	{
		throw MatrixError("Cannot add matrices with different col count");
	}

	Matrix<U> result{ lhs.n_rows(), lhs.n_cols() };

	for (const auto& [n_row, lhs_row] : enumerate(lhs.m_data))
	{
		for (const auto& [n_col, lhs_elem] : enumerate(lhs_row))
		{
			result(n_row, n_col) = lhs_elem - rhs(n_row, n_col);
		}
	}

	return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m)
{
	auto log_row = [&os](const auto& row)
	{
		os << "[";
		for (const auto& elem : row)
		{
			os << elem << ",";
		}
		os << "]" << '\n';
	};

	os << '\n';

	for (const auto& row : m.get_impl())
	{
		log_row(row);
	}

	return os;
}

template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c) : Matrix(r, c, T{ 0 })
{
}

template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c, T initial) : m_rows{ r }, m_cols{ c }
{
	if ((r == 0) || (c == 0))
	{
		throw MatrixError("Cannot construct matrix with zero row/col count");
	}

	if ((r < 0) || (c < 0))
	{
		throw MatrixError("Cannot construct matrix with negative row/col count");
	}

	m_data.resize(r);

	for (auto& row : m_data)
	{
		row.resize(c, initial);
	}
}

template <typename T>
std::size_t Matrix<T>::n_rows() const
{
	return m_rows;
}

template <typename T>
std::size_t Matrix<T>::n_cols() const
{
	return m_cols;
}

template <typename T>
auto Matrix<T>::get_impl() const
{
	return m_data;
}

template <typename T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix result{ n_cols(), n_rows()};

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			result(n_col, n_row) = elem;
		}
	}

	return result;
}
