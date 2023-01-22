#include "matrix_range.h"
#include "matrix_error.h"

Range::Range(std::size_t start, std::size_t end)
    : m_data{{start, end}}
{
    if (start > end)
    {
        throw MatrixError{"Invalid range: start > end"};
    }
}

Range::Range(std::size_t value)
    : Range(value, value)
{
}

std::size_t Range::start() const
{
    if (!m_data)
    {
        throw MatrixError{"Invalid range: Empty"};
    }
    else
    {
        return m_data.value().first;
    }
}

std::size_t Range::end() const
{
    if (!m_data)
    {
        throw MatrixError{"Invalid range: Empty"};
    }
    else
    {
        return m_data.value().second;
    }
}

std::size_t Range::size() const
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

Range::operator bool() const
{
    return m_data.has_value();
}