// asteroid.cpp

#include "asteroid.hpp"

void Asteroid::initialize(Vector2 pos, Vector2 sz, float rot,
    float rotSpeed, Color col, int lyr)
{
    position = pos;
    size = sz;
    rotation = rot;
    rotationSpeed = rotSpeed;
    color = col;
    layer = lyr;
}

void Asteroid::update()
{
    rotation += rotationSpeed;
    if (rotation > 360) rotation -= 360;
    if (rotation < 0) rotation += 360;
}