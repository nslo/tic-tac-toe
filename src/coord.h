#pragma once

#include <functional>

template <typename T>
struct Coord {
    Coord() : x(0), y(0)
    {
    }

    Coord(T _x, T _y) : x(_x), y(_y)
    {
    }

    T x, y;
};

// TODO: approximate equality
template <typename T>
inline bool operator==(const Coord<T>& lhs, const Coord<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace std
{
template <typename T>
struct hash<Coord<T>> {
    inline size_t operator()(const Coord<T>& coord) const
    {
        return std::hash<T>()(coord.x) ^ std::hash<T>()(coord.y);
    }
};
}
