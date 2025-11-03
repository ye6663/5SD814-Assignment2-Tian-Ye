// asteroid.hpp

#pragma once
#include "entity.hpp"
#include <raylib.h>

class Asteroid
{
public:
    void initialize(Vector2 position, Vector2 size, float rotation, float rotationSpeed, Color color, int layer, Entity entity, Vector2 worldSize, int generation = 0);
    void update();
    Rectangle getCollisionRect() const;
    bool canSplit() const { return m_generation < MAX_GENERATIONS; }
    std::vector<Asteroid> split();
    int getGeneration() const { return m_generation; }
    bool shouldRemove() const { return m_shouldRemove; }
    void markForRemoval() { m_shouldRemove = true; }
    void setVelocity(Vector2 velocity) { m_velocity = velocity; }
    const Entity& getEntity() const { return m_entity; }

private:
    Vector2 m_worldSize;
    Entity m_entity;
    float rotationSpeed;
    Vector2 m_velocity = { 0, 0 };
    int m_generation = 0;
    bool m_shouldRemove = false;
    static const int MAX_GENERATIONS = 2;
};