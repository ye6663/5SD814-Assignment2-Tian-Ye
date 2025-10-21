// math_utils.cpp

#include "math_utils.hpp"
#include <raylib.h>
#include <random>
#include <algorithm>

namespace MathUtils
{
    float random(float min, float max)
    {
        return min + static_cast<float>(GetRandomValue(0, 10000)) / 10000.0f * (max - min);
    }

    int random(int min, int max)
    {
        return GetRandomValue(min, max);
    }

    // Linear interpolation
    float lerp(float start, float end, float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return start + (end - start) * t;
    }

    // 2D vector linear interpolation
    struct Vector2 lerp(const struct Vector2& start, const struct Vector2& end, float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return {
            start.x + (end.x - start.x) * t,
            start.y + (end.y - start.y) * t
        };
    }

    float easeInOut(float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    float easeIn(float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return t * t;
    }

    float easeOut(float t)
    {
        t = clamp(t, 0.0f, 1.0f);
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    float clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
}