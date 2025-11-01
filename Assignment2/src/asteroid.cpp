// asteroid.cpp

#include "asteroid.hpp"

void Asteroid::initialize(Vector2 pos, Vector2 sz, float rot, float rotSpeed, Color col, int lyr, Entity entity, int generation)
{
    m_entity.set_position(pos);
    m_entity.set_rotation(rot);
    m_entity.set_texture(entity.get_sprite().get_texture());
    m_entity.set_tint(entity.get_sprite().get_tint());
    m_entity.set_layer(entity.get_layer());
    m_entity.set_size(sz);

    rotationSpeed = rotSpeed;
    m_generation = generation;
}

void Asteroid::update()
{
    float rotation = m_entity.get_transform().rotation;
    rotation += rotationSpeed;
    if (rotation > 360) rotation -= 360;
    if (rotation < 0) rotation += 360;

    m_entity.set_rotation(rotation);
}

Rectangle Asteroid::getCollisionRect() const
{
    Vector2 position = m_entity.get_transform().position;
    Vector2 size = m_entity.get_transform().size;
    return Rectangle{
        position.x - size.x / 2,
        position.y - size.y / 2,
        size.x,
        size.y
    };
}

void Asteroid::shrink()
{
    if (!canShrink()) {
        markForRemoval();
        return;
    }

    m_generation++;

    Vector2 size = m_entity.get_transform().size;
    switch (m_generation) {
    case 1:  // First reduction: Medium
        m_entity.get_transform().set_size({size.x * 0.6f, size.y * 0.6f});
        break;
    case 2:  // Second reduction: Small size
        m_entity.get_transform().set_size({ size.x * 0.6f, size.y * 0.6f });
        break;
    case 3:  // The third time: disappearing
        markForRemoval();
        return;
    default:
        markForRemoval();
        return;
    }
}