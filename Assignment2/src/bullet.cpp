// bullet.cpp

#include "bullet.hpp"
#include <raylib.h>
#include <cmath>

void Bullet::initialize(Vector2 position, Vector2 direction, float speed, float maxDistance, Entity entity)
{
    m_entity.set_position(position);
    m_entity.set_rotation(atan2f(direction.y, direction.x));
    m_entity.set_texture(entity.get_sprite().get_texture());
    m_entity.set_tint(entity.get_sprite().get_tint());
    m_entity.set_layer(entity.get_layer());
    m_entity.set_size(entity.get_transform().size);

    m_startPosition = position;
    m_direction = direction;
    m_speed = speed;
    m_maxDistance = maxDistance;
}

bool Bullet::update()
{
    // Moving bullets
    Vector2 position = m_entity.get_transform().position;
    position.x += m_direction.x * m_speed;
    position.y += m_direction.y * m_speed;

    // Check if the maximum distance has been exceeded
    float distance = sqrtf((position.x - m_startPosition.x) * (position.x - m_startPosition.x) +
        (position.y - m_startPosition.y) * (position.y - m_startPosition.y));
    if (distance > m_maxDistance)
    {
        return false; // Need to be destroyed
    }

    m_entity.set_position(position);
    return true;
}

Rectangle Bullet::getCollisionRect() const
{
    Vector2 position = m_entity.get_transform().position;
    Vector2 size = m_entity.get_transform().size;
    /*
    return Rectangle{
        position.x - size.x / 2,
        position.y - size.y / 2,
        size.x,
        size.y
    };
    */

    float collisionScale = 0.4f;
    Vector2 collisionSize = {
        size.x * collisionScale,
        size.y * collisionScale
    };

    return Rectangle{
        position.x - collisionSize.x / 2,
        position.y - collisionSize.y / 2,
        collisionSize.x,
        collisionSize.y
    };
}