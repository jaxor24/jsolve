#pragma once
#include <tuple>
#include <limits>
#include <cmath>

// Numeric tools

inline bool approx_equal(double a, double b)
{
    int factor = 2;
    double min_a = a - (a - std::nextafter(a, std::numeric_limits<double>::lowest())) * factor;
    double max_a = a + (std::nextafter(a, std::numeric_limits<double>::max()) - a) * factor;
    return min_a <= b && max_a >= b;
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
