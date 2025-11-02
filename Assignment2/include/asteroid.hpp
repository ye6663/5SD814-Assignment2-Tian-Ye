// asteroid.hpp

#pragma once
#include "entity.hpp"
#include <raylib.h>

class Asteroid
{
public:
    void initialize(Vector2 position, Vector2 size, float rotation, float rotationSpeed, Color color, int layer, Entity entity, int generation = 0);
    void update();
    Rectangle getCollisionRect() const;
    bool canSplit() const { return m_generation < MAX_GENERATIONS; }
    std::vector<Asteroid> split();
    int getGeneration() const { return m_generation; }
    bool shouldRemove() const { return m_shouldRemove; }
    void markForRemoval() { m_shouldRemove = true; }
    const Entity& getEntity() const { return m_entity; }

private:
    Entity m_entity;
    float rotationSpeed;
    int m_generation = 0;
    bool m_shouldRemove = false;
    static const int MAX_GENERATIONS = 2;
};