#include <vector>


template <typename T>
class MatrixIterator
{
public:
	using value_type = T;
	using value_type_no_const = typename std::remove_const<value_type>::type;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;

	using base_iterator_t = typename std::conditional_t<
		std::is_const<T>::value,
		typename std::vector<value_type_no_const>::const_iterator,
		typename std::vector<value_type_no_const>::iterator
	>;

	MatrixIterator(base_iterator_t curr, base_iterator_t end, std::size_t cols) 
		: 
		m_curr{ curr }, 
		m_end{ end }, 
		m_n_cols{ cols } 
	{}

	reference operator*() const { return *m_curr; }
	pointer operator->() { return m_curr; }

	MatrixIterator& operator++() { m_curr++; return *this; }
	MatrixIterator operator++(int) { const auto tmp = *this; ++(*this); return tmp; }

	bool operator== (const MatrixIterator& other) const { return m_curr == other.m_curr; }
	bool operator!= (const MatrixIterator& other) const { return !(*this == other); }

	MatrixIterator& next_row()
	{
		if (std::distance(m_curr, m_end) >= difference_type(m_n_cols))
		{
			std::advance(m_curr, m_n_cols);
		}
		else
		{
			m_curr = m_end;
		}
		return *this;
	}

	MatrixIterator& next_col()
	{
		if ((std::distance(m_curr, m_end) - 1) % m_n_cols == 0)
		{
			m_curr = m_end;
		}
		else
		{
			++m_curr;
		}
		return *this;
	}

private:
	base_iterator_t m_curr;
	base_iterator_t m_end;
	std::size_t m_n_cols;
};

template <typename T>
class RowIterator
{
public:
	using value_type = T;
	using value_type_no_const = typename std::remove_const<value_type>::type;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;

	using base_iterator_t = typename std::conditional<
		std::is_const<T>::value,
		typename std::vector<value_type_no_const>::const_iterator,
		typename std::vector<value_type_no_const>::iterator
	>::type;

	RowIterator(base_iterator_t curr, base_iterator_t end, std::size_t cols)
		:
		m_curr{ curr },
		m_end{ end },
		m_n_cols{ cols }
	{}

	reference operator*() const { return *m_curr; }
	pointer operator->() { return m_curr; }

	RowIterator& operator++()
	{ 
		next_row(); 
		return *this; 
	}

	RowIterator operator++(int)
	{ 
		const auto tmp = *this; 
		++(*this); 
		return tmp; 
	}

	bool operator== (const RowIterator& other) const { return m_curr == other.m_curr; }
	bool operator!= (const RowIterator& other) const { return !(*this == other); }

	RowIterator& next_row()
	{
		if (std::distance(m_curr, m_end) >= difference_type(m_n_cols))
		{
			std::advance(m_curr, m_n_cols);
		}
		else
		{
			m_curr = m_end;
		}
		return *this;
	}

	RowIterator& next_col()
	{
		if ((std::distance(m_curr, m_end) - 1) % m_n_cols == 0)
		{
			m_curr = m_end;
		}
		else
		{
			++m_curr;
		}
		return *this;
	}

private:
	base_iterator_t m_curr;
	base_iterator_t m_end;
	std::size_t m_n_cols;
};

template <typename T>
class ColIterator
{
public:
	using value_type = T;
	using value_type_no_const = typename std::remove_const<value_type>::type;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;

	using base_iterator_t = typename std::conditional<
		std::is_const<T>::value,
		typename std::vector<value_type_no_const>::const_iterator,
		typename std::vector<value_type_no_const>::iterator
	>::type;

	ColIterator(base_iterator_t curr, base_iterator_t end, std::size_t cols)
		:
		m_curr{ curr },
		m_end{ end },
		m_n_cols{ cols }
	{}

	reference operator*() const { return *m_curr; }
	pointer operator->() { return m_curr; }

	ColIterator& operator++()
	{
		next_col();
		return *this;
	}

	ColIterator operator++(int)
	{
		const auto tmp = *this;
		++(*this);
		return tmp;
	}

	bool operator== (const ColIterator& other) const { return m_curr == other.m_curr; }
	bool operator!= (const ColIterator& other) const { return !(*this == other); }

	ColIterator& next_row()
	{
		if (std::distance(m_curr, m_end) >= difference_type(m_n_cols))
		{
			std::advance(m_curr, m_n_cols);
		}
		else
		{
			m_curr = m_end;
		}
		return *this;
	}

	ColIterator& next_col()
	{
		if ((std::distance(m_curr, m_end) - 1) % m_n_cols == 0)
		{
			m_curr = m_end;
		}
		else
		{
			++m_curr;
		}
		return *this;
	}

private:
	base_iterator_t m_curr;
	base_iterator_t m_end;
	std::size_t m_n_cols;
};

template <typename It>
struct IterableWrapper
{
	// Provide an iterable object.

	struct Enumerator
	{
		// Wrap an interator and a counter.
		std::size_t m_counter;
		It m_iterator;

		bool operator!=(const Enumerator& other) const { return m_iterator != other.m_iterator; }
		void operator++() { ++m_counter; ++m_iterator; }
		auto operator*() const { return std::tie(m_counter, *m_iterator); }
	};

	It m_begin;
	It m_end;

	auto begin()
	{
		return Enumerator{ 0, m_begin };
	}

	auto end()
	{
		return Enumerator{ 0, m_end };
	}
};
