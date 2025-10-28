// player.cpp

#include "player.hpp"
#include <raylib.h>
#include <cmath>

void Player::initialize(Vector2 position)
{
    m_position = position;
    m_rotation = -PI / 2;
    m_velocity = { 0, 0 };

    m_texture = LoadTexture("assets/player_plane.png");
    float scale = 0.15f;
    m_size = {
        static_cast<float>(m_texture.width) * scale,
        static_cast<float>(m_texture.height) * scale
    };

    m_bullet_texture = LoadTexture("assets/bullet.png");
}

void Player::shutdown()
{
    UnloadTexture(m_texture);
}

void Player::setViewParameter(Vector2 worldSize, Rectangle cameraFrame)
{
    m_worldSize = worldSize;
    m_cameraFrame = cameraFrame;
}

void Player::update()
{
    // Apply drag
    m_velocity.x *= DRAG;
    m_velocity.y *= DRAG;

    // Limit Maximum Speed
    float speed = sqrtf(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    if (speed > MAX_SPEED)
    {
        m_velocity.x = m_velocity.x / speed * MAX_SPEED;
        m_velocity.y = m_velocity.y / speed * MAX_SPEED;
    }

    // Update position
    m_position.x += m_velocity.x;
    m_position.y += m_velocity.y;
    if (m_position.x - m_cameraFrame.width < 0) m_position.x = m_cameraFrame.width;
    if (m_position.y - m_cameraFrame.height < 0) m_position.y = m_cameraFrame.height;
    if (m_position.x + m_cameraFrame.width > m_worldSize.x) m_position.x = m_worldSize.x - m_cameraFrame.width;
    if (m_position.y + m_cameraFrame.height > m_worldSize.y) m_position.y = m_worldSize.y - m_cameraFrame.height;
}

void Player::applyThrust()
{
    m_velocity.x += cosf(m_rotation) * THRUST_FORCE;
    m_velocity.y += sinf(m_rotation) * THRUST_FORCE;
}

void Player::rotateLeft()
{
    m_rotation -= ROTATION_SPEED * GetFrameTime();
}

void Player::rotateRight()
{
    m_rotation += ROTATION_SPEED * GetFrameTime();
}

void Player::shoot()
{
    double currentTime = GetTime();
    if (currentTime - m_lastShotTime < FIRE_RATE) {
        return;
    }
    m_lastShotTime = currentTime;

    Bullet bullet;
    Vector2 direction = { cosf(m_rotation), sinf(m_rotation) };
    bullet.initialize(m_position, direction, BULLET_SPEED, BULLET_MAX_DISTANCE, m_bullet_texture);
    m_bullets.push_back(bullet);
}