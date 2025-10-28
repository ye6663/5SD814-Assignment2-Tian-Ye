// bullet.cpp

#include "bullet.hpp"
#include <raylib.h>
#include <cmath>

void Bullet::initialize(Vector2 position, Vector2 direction, float speed, float maxDistance, Texture2D texture)
{
    m_startPosition = position;
    m_position = position;
    m_direction = direction;
    m_speed = speed;
    m_maxDistance = maxDistance;

    m_texture = texture;
    float scale = 0.5f;
    m_size = {
        static_cast<float>(m_texture.width) * scale,
        static_cast<float>(m_texture.height) * scale
    };
}

bool Bullet::update()
{
    // Moving bullets
    m_position.x += m_direction.x * m_speed;
    m_position.y += m_direction.y * m_speed;

    // Check if the maximum distance has been exceeded
    float distance = sqrtf((m_position.x - m_startPosition.x) * (m_position.x - m_startPosition.x) +
        (m_position.y - m_startPosition.y) * (m_position.y - m_startPosition.y));
    if (distance > m_maxDistance)
    {
        return false; // Need to be destroyed
    }

    return true;
}

Rectangle Bullet::getCollisionRect() const
{
    return Rectangle{
        m_position.x - m_size.x / 2,
        m_position.y - m_size.y / 2,
        m_size.x,
        m_size.y
    };
}