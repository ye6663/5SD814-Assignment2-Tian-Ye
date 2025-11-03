// asteroid.cpp

#include "asteroid.hpp"
#include "math_utils.hpp"

void Asteroid::initialize(Vector2 pos, Vector2 sz, float rot, float rotSpeed, Color col, int lyr, Entity entity, Vector2 worldSize, int generation)
{
    m_worldSize = worldSize;
    m_entity.set_position(pos);
    m_entity.set_rotation(rot);
    m_entity.set_texture(entity.get_sprite().get_texture());
    m_entity.set_tint(entity.get_sprite().get_tint());
    m_entity.set_layer(entity.get_layer());
    m_entity.set_size(sz);

    rotationSpeed = rotSpeed;
    m_generation = generation;

    if (MathUtils::random(0, 5) < 2) {
        float speed = MathUtils::random(0.5f, 2.0f);
        float angle = MathUtils::random(0.0f, 2 * PI);
        m_velocity = {
            cosf(angle) * speed,
            sinf(angle) * speed
        };
    }
}

void Asteroid::update()
{
    float rotation = m_entity.get_transform().rotation;
    rotation += rotationSpeed;
    if (rotation > 360) rotation -= 360;
    if (rotation < 0) rotation += 360;

    m_entity.set_rotation(rotation);

    if (m_velocity.x != 0 || m_velocity.y != 0) {
        Vector2 position = m_entity.get_transform().position;
        position.x += m_velocity.x;
        position.y += m_velocity.y;

        if (position.x < 0) position.x = m_worldSize.x;
        else if (position.x > m_worldSize.x) position.x = 0;

        if (position.y < 0) position.y = m_worldSize.y;
        else if (position.y > m_worldSize.y) position.y = 0;

        m_entity.set_position(position);
    }
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

std::vector<Asteroid> Asteroid::split()
{
    std::vector<Asteroid> fragments;

    markForRemoval();
    if (!canSplit()) {
        return fragments;
    }

    int splitNumber = m_generation == 0 ? 1 : 2;
    int fragmentCount = splitNumber + MathUtils::random(0, 1);
    for (int i = 0; i < fragmentCount; i++) {
        Asteroid fragment;

        Vector2 position = m_entity.get_transform().position;
        float rotation = m_entity.get_transform().rotation;
        Vector2 size = m_entity.get_transform().size;
        Vector2 newSize = {
            size.x * 0.6f,
            size.y * 0.6f
        };

        Vector2 newPosition;
        if (fragmentCount != 1) {
            float offsetDistance = size.x * 0.4f;
            float angle = (float)i / fragmentCount * 2 * PI + MathUtils::random(-0.5f, 0.5f);
            Vector2 offset = {
                cosf(angle) * offsetDistance,
                sinf(angle) * offsetDistance
            };
            newPosition = {
                position.x + offset.x,
                position.y + offset.y
            };
        }
        else {
            newPosition = position;
        }

        fragment.initialize(newPosition, newSize, rotation, rotationSpeed, m_entity.get_sprite().get_tint(), m_entity.get_layer() - 1, m_entity, m_worldSize, m_generation + 1);
        fragments.push_back(fragment);
    }

    return fragments;
}