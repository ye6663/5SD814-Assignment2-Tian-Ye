// transform.cpp

#include "transform.hpp"

transform::transform(Vector2 pos)
    : position(pos)
{
}

void transform::set_size(Vector2 s)
{
    this->size = s;
}

void transform::set_position(Vector2 pos)
{
    this->position = pos;
}

void transform::set_rotation(float rot)
{
    this->rotation = rot;
}