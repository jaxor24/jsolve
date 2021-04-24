#pragma once

#include <vector>
#include <iostream>

template <typename T>
class Matrix
{
public:
	Matrix(unsigned r, unsigned c);
	Matrix(unsigned r, unsigned c, T initial);

	unsigned rows() const;
	unsigned cols() const;

	auto get_impl() const;

private:
	unsigned m_rows;
	unsigned m_cols;
	std::vector<std::vector<T>> m_data;
};


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

	os << std::endl;
	for (const auto& row : m.get_impl())
	{
		log_row(row);
	}

	return os;
}

template <typename T>
Matrix<T>::Matrix(unsigned r, unsigned c) : Matrix(r, c, T{ 0 }) 
{
}

template <typename T>
Matrix<T>::Matrix(unsigned r, unsigned c, T initial) : m_rows{ r }, m_cols{ c }
{
	m_data.resize(r);
	for (auto& row : m_data)
	{
		row.resize(c, initial);
	}
}

template <typename T>
unsigned Matrix<T>::rows() const
{
	return m_rows;
}

template <typename T>
unsigned Matrix<T>::cols() const
{
	return m_cols;
}

template <typename T>
auto Matrix<T>::get_impl() const
{
	return m_data;
}
