// transform.hpp

#pragma once
#include <raylib.h>

struct transform {
    transform() = default;
    transform(Vector2 pos);

    void set_size(Vector2 s);
    void set_position(Vector2 pos);
    void set_rotation(float rot);

    Vector2 size = {60, 60};
    Vector2 position = {};
    float rotation = 0.0f;
};