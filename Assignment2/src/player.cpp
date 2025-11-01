// player.cpp

#include "player.hpp"
#include "event_system.hpp"
#include <raylib.h>
#include <cmath>

void Player::initialize(Vector2 position, TextureManager& textureManager)
{
    m_velocity = { 0, 0 };
    m_entity.set_position(position);
    m_entity.set_rotation(-PI / 2);
    m_entity.set_layer(10);

    this->m_textureManager = &textureManager;
    auto textureHandle = textureManager.load("player");
    if (textureHandle.index != uint16_t(-1)) {
        m_entity.set_texture(textureHandle);

        Texture2D* texturePtr = textureManager.get_texture_from_handle(textureHandle);
        if (texturePtr) {
            float scale = 0.15f;
            m_entity.set_size({
                static_cast<float>(texturePtr->width) * scale,
                static_cast<float>(texturePtr->height) * scale
            });
        }
    }

    auto bulletHandle = textureManager.load("bullet");
    if (bulletHandle.index != uint16_t(-1)) {
        m_bullet_entity.set_texture(bulletHandle);
        Texture2D* texturePtr = textureManager.get_texture_from_handle(bulletHandle);
        if (texturePtr) {
            float scale = 0.5f;
            m_bullet_entity.set_size({
                static_cast<float>(texturePtr->width) * scale,
                static_cast<float>(texturePtr->height) * scale
                });
        }
    }
}

void Player::shutdown()
{
    this->m_textureManager->unload(m_entity.get_sprite().get_texture());
}

TextureHandle Player::getTexture() const {
    TextureHandle handle = m_entity.get_sprite().get_texture();
    return handle;
}

void Player::setViewParameter(Vector2 worldSize, Rectangle cameraFrame)
{
    m_worldSize = worldSize;
    m_cameraFrame = cameraFrame;
}

void Player::update()
{
    m_entity.set_position(m_entity.get_transform().position);

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
    Vector2 position = m_entity.get_transform().position;
    position.x += m_velocity.x;
    position.y += m_velocity.y;
    if (position.x - m_cameraFrame.width < 0) position.x = m_cameraFrame.width;
    if (position.y - m_cameraFrame.height < 0) position.y = m_cameraFrame.height;
    if (position.x + m_cameraFrame.width > m_worldSize.x) position.x = m_worldSize.x - m_cameraFrame.width;
    if (position.y + m_cameraFrame.height > m_worldSize.y) position.y = m_worldSize.y - m_cameraFrame.height;

    m_entity.set_position(position);
}

void Player::applyThrust()
{
    float currentRotation = m_entity.get_transform().rotation;
    m_velocity.x += cosf(currentRotation) * THRUST_FORCE;
    m_velocity.y += sinf(currentRotation) * THRUST_FORCE;
}

void Player::rotateLeft()
{
    float currentRotation = m_entity.get_transform().rotation;
    float newRotation = currentRotation - ROTATION_SPEED * GetFrameTime();
    m_entity.set_rotation(newRotation);
}

void Player::rotateRight()
{
    float currentRotation = m_entity.get_transform().rotation;
    float newRotation = currentRotation + ROTATION_SPEED * GetFrameTime();
    m_entity.set_rotation(newRotation);
}

void Player::shoot()
{
    double currentTime = GetTime();
    if (currentTime - m_lastShotTime < FIRE_RATE) {
        return;
    }
    m_lastShotTime = currentTime;

    float rotation = m_entity.get_transform().rotation;
    Bullet bullet;
    Vector2 direction = { cosf(rotation), sinf(rotation) };
    bullet.initialize(m_entity.get_transform().position, direction, BULLET_SPEED, BULLET_MAX_DISTANCE, m_bullet_entity);
    m_bullets.push_back(bullet);

    // 发布射击事件
    EventSystem::getInstance().publish(EventType::PlayerShoot);
}