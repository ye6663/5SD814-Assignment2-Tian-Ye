// bullet.hpp

#pragma once
#include "entity.hpp"
#include <raylib.h>
#include <cmath>

class Bullet
{
public:
    void initialize(Vector2 position, Vector2 direction, float speed, float maxDistance, Entity entity);
    bool update(); // Return false to indicate that the bullet should be destroyed
    Rectangle getCollisionRect() const;

    Vector2 getPosition() const { return m_entity.get_transform().position; }
    Vector2 getDirection() const { return m_direction; }
    Vector2 getSize() const { return m_entity.get_transform().size; }
    float getRotation() const { return atan2f(m_direction.y, m_direction.x); }
    const Entity& getEntity() const { return m_entity; }

private:
    Entity m_entity;
    TextureHandle* m_textureHandle;
    Vector2 m_startPosition;
    Vector2 m_direction;
    float m_speed;
    float m_maxDistance;
};