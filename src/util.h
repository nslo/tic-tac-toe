#pragma once

#include <algorithm>

template <typename T>
inline T clamp(T val, T min, T max)
{
    return std::min(max, std::max(min, val));
}
