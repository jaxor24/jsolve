#include <tuple>
#include <vector>

template <typename It>
struct IteratorConverter;

template <typename Increment, typename T>
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
        std::is_const<T>::value, typename std::vector<value_type_no_const>::const_iterator,
        typename std::vector<value_type_no_const>::iterator>;

    MatrixIterator(base_iterator_t curr, base_iterator_t end, std::size_t cols)
        : m_curr{curr},
          m_end{end},
          m_n_cols{cols}
    {
    }

    reference operator*() const
    {
        return *m_curr;
    }

    pointer operator->()
    {
        return m_curr;
    }

    MatrixIterator& operator++()
    {
        static_cast<Increment*>(this)->increment();
        return *this;
    }

    MatrixIterator operator++(int)
    {
        const auto tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const MatrixIterator& other) const
    {
        return m_curr == other.m_curr;
    }

    bool operator!=(const MatrixIterator& other) const
    {
        return !(*this == other);
    }

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

  protected:
    base_iterator_t m_curr;
    base_iterator_t m_end;
    std::size_t m_n_cols;

    template <typename Input, typename Output>
    friend Output make_iterator(const Input& it, bool move_to_end);
};

template <typename T>
class AllIterator : public MatrixIterator<AllIterator<T>, T>
{
    // Alias needed to avoid name-hiding:
    // https://www.reddit.com/r/cpp_questions/comments/5vtoej/overloading_parents_template_function_in_child/

    using MatrixIterator<AllIterator<T>, T>::MatrixIterator;

  public:
    void increment()
    {
        this->m_curr++;
    }
};
