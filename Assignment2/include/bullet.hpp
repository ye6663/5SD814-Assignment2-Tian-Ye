// bullet.hpp

#pragma once
#include <raylib.h>
#include <cmath>

class Bullet
{
public:
    void initialize(Vector2 position, Vector2 direction, float speed, float maxDistance, Texture2D texture);
    bool update(); // Return false to indicate that the bullet should be destroyed
    Rectangle getCollisionRect() const;

    Vector2 getPosition() const { return m_position; }
    Vector2 getDirection() const { return m_direction; }
    Texture2D getTexture() const { return m_texture; }
    Vector2 getSize() const { return m_size; }
    float getRotation() const { return atan2f(m_direction.y, m_direction.x); }

private:
    Vector2 m_startPosition;
    Vector2 m_position;
    Vector2 m_direction;
    float m_speed;
    float m_maxDistance;

    Texture2D m_texture;
    Vector2 m_size;
};