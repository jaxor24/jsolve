#pragma once

template <typename T>
struct counter
{
    static inline int objects_created = 0;
    static inline int objects_alive = 0;

    counter()
    {
        ++objects_created;
        ++objects_alive;
    }

    counter(const counter&)
    {
        ++objects_created;
        ++objects_alive;
    }

protected:
    ~counter()
    {
        --objects_alive;
    }
};

template <typename T>
bool operator<(const counter<T>& a, const counter<T>& b)
{
    return a.objects_created < b.objects_created;
}