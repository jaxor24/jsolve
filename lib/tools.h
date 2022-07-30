#pragma once

#include "logging.h"

#include <tuple>
#include <limits>
#include <cmath>
#include <chrono>
#include <string>

// Basic RAII timer

template <typename Log, typename... Args>
class Timer
{
public:
    using Clock = std::chrono::high_resolution_clock;

    Timer(Log log, fmt::format_string<Args...> format, Args... args)
        : 
        m_start{ Clock::now() },
        m_log{ log },
        m_msg{ fmt::format(format, std::forward<Args>(args)...) }
    {
        m_log("(Start) {}", m_msg);
    }

    ~Timer()
    {
        auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_start).count();
        m_log("(End) {} ({} ms)", m_msg, dur);
    }

private:
    const Clock::time_point m_start;
    Log m_log;
    const std::string m_msg;

};

// Numeric tools

inline bool approx_equal(double a, double b, double eps = 1e-8)
{
    // Compares a == b, where they will be considered equal below some tolerance.
    // Compares infinity to itself as true.

    if (a == std::numeric_limits<double>::infinity() && b == std::numeric_limits<double>::infinity())
    {
        return true;
    }
    else
    {
        return fabs(a - b) <= eps;
    }
}

inline bool approx_greater(double a, double b, double eps = 1e-8)
{
    // Compares a > b, where they will be considered equal below some tolerance.
    return a > b && !approx_equal(a, b, eps);
}


// Python-like enumerate()
// Ref: http://reedbeta.com/blog/python-like-enumerate-in-cpp17/

template<typename Iterable>
auto enumerate(Iterable&& iterable) 
{
    using Iterator = decltype(std::begin(std::declval<Iterable>()));
    using T = decltype(*std::declval<Iterator>());

    struct Enumerated 
    {
        std::size_t index;
        T element;
    };

    struct Enumerator 
    {
        Iterator iterator;
        std::size_t index;

        auto operator!=(const Enumerator& other) const 
        {
            return iterator != other.iterator;
        }

        auto& operator++() 
        {
            ++iterator;
            ++index;
            return *this;
        }

        auto operator*() const 
        {
            return Enumerated{ index, *iterator };
        }
    };

    struct Wrapper 
    {
        Iterable& iterable;

        [[nodiscard]] auto begin() const 
        {
            return Enumerator{ std::begin(iterable), 0U };
        }

        [[nodiscard]] auto end() const 
        {
            return Enumerator{ std::end(iterable), 0U };
        }
    };

    return Wrapper{ std::forward<Iterable>(iterable) };
}
