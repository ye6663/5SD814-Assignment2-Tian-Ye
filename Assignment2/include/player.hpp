// player.hpp

#pragma once
#include <raylib.h>
#include "bullet.hpp"
#include <vector>

class Player
{
public:
    void initialize(Vector2 position);
    void shutdown();
    void setViewParameter(Vector2 worldSize, Rectangle cameraFrame);
    void update();
    
    void applyThrust();
    void rotateLeft();
    void rotateRight();
    void shoot();
    
    Vector2 getPosition() const { return m_position; }
    float getRotation() const { return m_rotation; }
    Texture2D getTexture() const { return m_texture; }
    Vector2 getSize() const { return m_size; }

    std::vector<Bullet>& getBullets() { return m_bullets; }
    
private:
    Vector2 m_position = {0, 0};
    Vector2 m_velocity = {0, 0};
    float m_rotation = 0;
    Vector2 m_worldSize;
    Rectangle m_cameraFrame;
    
    const float ROTATION_SPEED = 4.0f;
    const float THRUST_FORCE = 0.2f;
    const float DRAG = 0.98f;
    const float MAX_SPEED = 10.0f;

    const float BULLET_SPEED = 12.0f; // Bullet speed
    const float BULLET_MAX_DISTANCE = 3000.0f; // Maximum distance of bullets
    const double FIRE_RATE = 0.2;

    Texture2D m_texture;
    Vector2 m_size = { 0, 0 };
    Texture2D m_bullet_texture;
    double m_lastShotTime = 0.0;
    std::vector<Bullet> m_bullets;
};