#pragma once

#include <tuple>
#include <limits>
#include <cmath>
#include <chrono>
#include <string>

// Basic RAII timer

template <typename Log>
class Timer
{
public:
    using Clock = std::chrono::high_resolution_clock;
    Timer(Log log, const std::string& msg) 
        : 
        m_start{ Clock::now() },
        m_log{ log },
        m_msg{ msg }
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
    if (a == std::numeric_limits<double>::infinity() && b == std::numeric_limits<double>::infinity())
    {
        return true;
    }
    else
    {
        return fabs(a - b) <= eps;
    }
}


// Python-like enumerate()
// Ref: http://reedbeta.com/blog/python-like-enumerate-in-cpp17/

template <typename T,
typename TIter = decltype(std::begin(std::declval<T>())),
typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable)
{
    struct iterator
    {
        size_t i;
        TIter iter;
        bool operator != (const iterator& other) const { return iter != other.iter; }
        void operator ++ () { ++i; ++iter; }
        auto operator * () const { return std::tie(i, *iter); }
    };
    struct iterable_wrapper
    {
        T iterable;
        auto begin() { return iterator{ 0, std::begin(iterable) }; }
        auto end() { return iterator{ 0, std::end(iterable) }; }
    };
    return iterable_wrapper{ std::forward<T>(iterable) };
}
