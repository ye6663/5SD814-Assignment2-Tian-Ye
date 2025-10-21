// player.cpp

#include "player.hpp"
#include <raylib.h>
#include <cmath>

void Player::initialize(Vector2 position)
{
    m_position = position;
    m_rotation = 0;
    m_velocity = { 0, 0 };
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