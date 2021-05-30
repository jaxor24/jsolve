#pragma once

#include "tools.h"
#include "logging.h"

#include <vector>
#include <optional>
#include <iostream>
#include <stdexcept>


class MatrixError : public std::runtime_error
{
public:
	template <typename... Args>
	explicit MatrixError(Args&&... args) 
		: 
		std::runtime_error(fmt::format(std::forward<Args>(args)...)) 
	{}
	virtual ~MatrixError() throw () {}
};

template <typename T>
class Matrix
{
public:
	typedef T value_type;

	class Range
	{
	public:
		Range() = default;
		Range(std::size_t start, std::size_t end);

		std::size_t start() const;
		std::size_t end() const;
		std::size_t size() const;

		operator bool() const;

	private:
		std::optional<std::pair<std::size_t, std::size_t>> m_data;
	};

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

	// Operators -------------------------------------------------------------------------------
	// Put-to
	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

	// Comparison
	template <typename U>
	friend bool operator==(const Matrix<U>& lhs, const Matrix<U>& rhs);
	template <typename U>
	friend bool operator!=(const Matrix<U>& lhs, const Matrix<U>& rhs);

	// Access
	T& operator()(const std::size_t row, const std::size_t col);
	T operator()(const std::size_t row, const std::size_t col) const;

	// Unary
	Matrix& operator++()
	{
		for_each_element([](value_type& elem) { elem++; });
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
		for_each_element([](value_type& elem) { elem--; });
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
		return *this;
	}
	
	// Binary - Scalar
	Matrix operator+=(double x)
	{
		for_each_element([x](value_type& elem) { elem += x; });
		return *this;
	}
	Matrix operator-=(double x)
	{
		for_each_element([x](value_type& elem) { elem -= x; });
		return *this;
	}
	Matrix operator*=(double x)
	{
		for_each_element([x](value_type& elem) { elem *= x; });
		return *this;
	}
	Matrix operator/=(double x)
	{
		if (x == 0)
		{
			throw MatrixError("Scalar division by zero");
		}

		for_each_element([x](value_type& elem) { elem /= x; });
		return *this;
	}

private:
	std::vector<std::vector<T>> m_data; // vector of rows

	template <typename F>
	void for_each_element(F f)
	{
		for (auto& row : m_data)
		{
			for (auto& elem : row)
			{
				f(elem);
			}
		}
	}

	template <typename F>
	void for_each_element(F f) const
	{
		for (const auto& row : m_data)
		{
			for (const auto& elem : row)
			{
				f(elem);
			}
		}
	}
};

// Matrix::Range
template <typename T>
Matrix<T>::Range::Range(std::size_t start, std::size_t end) 
	: 
	m_data{ { start, end } }
{
	if (start > end)	
	{ 
		throw MatrixError{ "Invalid range: start > end" };
	}
}

template <typename T>
std::size_t Matrix<T>::Range::start() const
{
	if (!m_data)
	{
		throw MatrixError{ "Invalid range: Empty" };
	}
	else
	{
		return m_data.value().first;
	}
}

template <typename T>
std::size_t Matrix<T>::Range::end() const
{
	if (!m_data)
	{
		throw MatrixError{ "Invalid range: Empty" };
	}
	else
	{
		return m_data.value().second;
	}
}

template <typename T>
std::size_t Matrix<T>::Range::size() const
{
	if (!m_data)
	{
		return 0;
	}
	else
	{
		// Range from 4 to 5 includes rows 4 and 5, hence size + 1
		return m_data.value().second - m_data.value().first + 1;
	}
}

template <typename T>
Matrix<T>::Range::operator bool() const 
{ 
	return m_data.has_value(); 
}

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
	if (lhs.n_rows() != rhs.n_rows())
	{
		return false;
	}

	if (lhs.n_cols() != rhs.n_cols())
	{
		return false;
	}

	for (const auto& [n_row, lhs_row] : enumerate(lhs.m_data))
	{
		for (const auto& [n_col, lhs_elem] : enumerate(lhs_row))
		{
			if (lhs(n_row, n_col) != rhs(n_row, n_col))
			{
				return false;
			}
		}
	}

	return true;
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

	for (const auto& row : m.m_data)
	{
		log_row(row);
	}

	return os;
}

// Matrix:: member functions
template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c) : Matrix(r, c, T{ 0 })
{
}

template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c, T initial)
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
T& Matrix<T>::operator()(const std::size_t row, const std::size_t col)
{
	if (row >= n_rows() || col >= n_cols())
	{
		throw MatrixError("Cannot access out of bounds");
	}

	return m_data[row][col];
}

template <typename T>
T Matrix<T>::operator()(const std::size_t row, const std::size_t col) const
{
	if (row >= n_rows() || col >= n_cols())
	{
		throw MatrixError("Cannot access out of bounds");
	}

	return m_data[row][col];
}

template <typename T>
std::size_t Matrix<T>::n_rows() const
{
	return m_data.size();
}

template <typename T>
std::size_t Matrix<T>::n_cols() const
{
	return m_data.front().size();
}

template <typename T>
T Matrix<T>::max() const
{
	std::optional<T> result;

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			result = result.has_value() ? std::max(result.value(), elem) : elem;
		}
	}

	return result.value();
}

template <typename T>
T Matrix<T>::min() const
{
	std::optional<T> result;

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			result = result.has_value() ? std::min(result.value(), elem) : elem;
		}
	}

	return result.value();
}

template <typename T>
T Matrix<T>::sum() const
{
	std::optional<T> result;

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			result = result.has_value() ? result.value() += elem : elem;
		}
	}

	return result.value();
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::row_max() const
{
	Matrix<T> values{ n_rows(), 1, std::numeric_limits<T>::lowest() };
	Matrix<std::size_t> indices{ n_rows(), 1, 0 };

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			if (elem > values(n_row, 0))
			{
				values(n_row, 0) = elem;
				indices(n_row, 0) = n_col;
			}
		}
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::row_min() const
{
	Matrix<T> values{ n_rows(), 1, std::numeric_limits<T>::max() };
	Matrix<std::size_t> indices{ n_rows(), 1, 0 };

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			if (elem < values(n_row, 0))
			{
				values(n_row, 0) = elem;
				indices(n_row, 0) = n_col;
			}
		}
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::col_max() const
{
	Matrix<T> values{ 1, n_cols(), std::numeric_limits<T>::lowest() };
	Matrix<std::size_t> indices{ 1, n_cols(), 0 };

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			if (elem > values(0, n_col))
			{
				values(0, n_col) = elem;
				indices(0, n_col) = n_row;
			}
		}
	}

	return { values, indices };
}

template <typename T>
std::pair<Matrix<T>, Matrix<std::size_t>> Matrix<T>::col_min() const
{
	Matrix<T> values{ 1, n_cols(), std::numeric_limits<T>::max() };
	Matrix<std::size_t> indices{ 1, n_cols(), 0 };

	for (const auto& [n_row, row] : enumerate(m_data))
	{
		for (const auto& [n_col, elem] : enumerate(row))
		{
			if (elem < values(0, n_col))
			{
				values(0, n_col) = elem;
				indices(0, n_col) = n_row;
			}
		}
	}

	return { values, indices };
}

template <typename T>
Matrix<T> Matrix<T>::make_transpose() const
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
	for (const auto& [n_row, row] : enumerate(m_data))
	{
		if (!rows || ((n_row >= rows.start()) && (n_row <= rows.end())))
		{
			std::size_t result_col{ 0 };
			for (const auto& [n_col, elem] : enumerate(row))
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
