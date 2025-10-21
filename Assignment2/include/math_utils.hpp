// math_utils.hpp

#pragma once
#include <raylib.h>

namespace MathUtils
{
    float random(float min, float max);
    int random(int min, int max);

    float lerp(float start, float end, float t);

    struct Vector2 lerp(const struct Vector2& start, const struct Vector2& end, float t);

    float easeInOut(float t);
    float easeIn(float t);
    float easeOut(float t);

    float clamp(float value, float min, float max);
};