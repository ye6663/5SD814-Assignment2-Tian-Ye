// asteroid.hpp

#pragma once
#include <raylib.h>

class Asteroid
{
public:
    void initialize(Vector2 position, Vector2 size, float rotation, 
                   float rotationSpeed, Color color, int layer);
    void update();
    
    Vector2 position;
    Vector2 size;
    float rotation;
    float rotationSpeed;
    Color color;
    int layer;
};