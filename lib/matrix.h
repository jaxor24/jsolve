#pragma once

#include "matrix_error.h"
#include "matrix_iterator.h"
#include "matrix_range.h"

#include "logging.h"
#include "tools.h"

#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <utility>
#include <vector>

template <typename T>
class Matrix
{
  public:
    typedef T value_type;
    typedef AllIterator<T> matrix_iterator;
    typedef AllIterator<const T> const_matrix_iterator;

    explicit Matrix(std::size_t r, std::size_t c);
    explicit Matrix(std::size_t r, std::size_t c, T initial);

    std::size_t n_rows() const;
    std::size_t n_cols() const;

    T min() const;
    Matrix make_transpose() const;

    Matrix slice(Range rows, Range cols) const;

    void update(Range rows, Range cols, const Matrix& values);

    // Iterators -------------------------------------------------------------------------------

    matrix_iterator begin();
    matrix_iterator end();

    const_matrix_iterator begin() const;
    const_matrix_iterator end() const;

    const_matrix_iterator cbegin() const;
    const_matrix_iterator cend() const;

    // Operators -------------------------------------------------------------------------------

    // Access
    T& operator()(const std::size_t row, const std::size_t col);
    const T& operator()(const std::size_t row, const std::size_t col) const;

    // Put-to
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& m);

    // Comparison
    template <typename U>
    friend bool operator==(const Matrix<U>& lhs, const Matrix<U>& rhs);
    template <typename U>
    friend bool operator!=(const Matrix<U>& lhs, const Matrix<U>& rhs);

    // Unary
    Matrix operator-();
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
    std::size_t m_n_rows{0};
    std::size_t m_n_cols{0};
};

#include "matrix_operator.hpp"

// Matrix:: member functions
template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c)
    : Matrix(r, c, T{0})
{
}

template <typename T>
Matrix<T>::Matrix(std::size_t r, std::size_t c, T initial)
    : m_n_rows{r},
      m_n_cols{c}
{
    if ((r == 0) || (c == 0))
    {
        throw MatrixError("Cannot construct matrix with zero row/col count");
    }

    m_data.resize(r * c, initial);
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
T Matrix<T>::min() const
{
    return *std::min_element(std::begin(m_data), std::end(m_data));
}
template <typename T>
Matrix<T> Matrix<T>::make_transpose() const
{
    Matrix result{n_cols(), n_rows()};

    for (std::size_t i{0}; i < n_rows(); ++i)
    {
        for (std::size_t j{0}; j < n_cols(); ++j)
        {
            result(j, i) = operator()(i, j);
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

    Matrix result{rows ? rows.size() : n_rows(), cols ? cols.size() : n_cols(), 0.0};

    std::size_t row_start = rows ? rows.start() : 0;
    std::size_t row_end = rows ? rows.end() : n_rows() - 1;
    std::size_t col_start = cols ? cols.start() : 0;
    std::size_t col_end = cols ? cols.end() : n_cols() - 1;

    for (std::size_t curr_row{row_start}; curr_row <= row_end; curr_row++)
    {
        for (std::size_t curr_col{col_start}; curr_col <= col_end; curr_col++)
        {
            result(curr_row - row_start, curr_col - col_start) = operator()(curr_row, curr_col);
        }
    }

    return result;
}

template <typename T>
void Matrix<T>::update(Range rows, Range cols, const Matrix& sub)
{
    // Update the specified range with the values in the provided sub matrix.

    // Check inputs
    if (rows)
    {
        // Rows must match sub matrix size
        if (rows.size() != sub.n_rows())
        {
            throw MatrixError(
                fmt::format("Row range of size is inconsistent with sub matrix", rows.size(), sub.n_rows())
            );
        }
    }
    else
    {
        // Rows must match target matrix size
        if (n_rows() != sub.n_rows())
        {
            throw MatrixError(
                fmt::format("Cannot update all {} rows with a sub-matrix of {} rows", n_rows(), sub.n_rows())
            );
        }
    }

    if (cols)
    {
        // Cols must match sub matrix size
        if (cols.size() != sub.n_cols())
        {
            throw MatrixError(
                fmt::format("Col range of size is inconsistent with sub matrix", cols.size(), sub.n_cols())
            );
        }
    }
    else
    {
        // Cols must match target matrix size
        if (n_cols() != sub.n_cols())
        {
            throw MatrixError(
                fmt::format("Cannot update all {} cols with a sub-matrix of {} cols", n_cols(), sub.n_cols())
            );
        }
    }

    // Shortcut for replacing entire matrix
    if (!rows && !cols)
    {
        *this = sub;
        return;
    }

    std::size_t row_start = rows ? rows.start() : 0;
    std::size_t row_end = rows ? rows.end() : n_rows() - 1;
    std::size_t col_start = cols ? cols.start() : 0;
    std::size_t col_end = cols ? cols.end() : n_cols() - 1;

    for (std::size_t curr_row{row_start}; curr_row <= row_end; curr_row++)
    {
        for (std::size_t curr_col{col_start}; curr_col <= col_end; curr_col++)
        {
            operator()(curr_row, curr_col) = sub(curr_row - row_start, curr_col - col_start);
        }
    }
}

template <typename T>
typename Matrix<T>::matrix_iterator Matrix<T>::begin()
{
    return {std::begin(m_data), std::end(m_data), n_cols()};
}

template <typename T>
typename Matrix<T>::matrix_iterator Matrix<T>::end()
{
    return {std::end(m_data), std::end(m_data), n_cols()};
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::begin() const
{
    return cbegin();
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::end() const
{
    return cend();
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::cbegin() const
{
    return {std::begin(m_data), std::end(m_data), n_cols()};
}

template <typename T>
typename Matrix<T>::const_matrix_iterator Matrix<T>::cend() const
{
    return {std::end(m_data), std::end(m_data), n_cols()};
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
