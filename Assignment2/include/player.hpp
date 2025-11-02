// player.hpp

#pragma once
#include "texture_manager.hpp"
#include "bullet.hpp"
#include "entity.hpp"
#include <raylib.h>
#include <vector>

class Player
{
public:
    void initialize(Vector2 position, TextureManager& textureManager);
    void shutdown();
    void setViewParameter(Vector2 worldSize, Rectangle cameraFrame);
    void update();
    
    void applyThrust();
    void rotateLeft();
    void rotateRight();
    void shoot();
    
    Vector2 getPosition() const { return m_entity.get_transform().position; }
    float getRotation() const { return m_entity.get_transform().rotation; }
    TextureHandle getTexture() const;
    Vector2 getSize() const { return m_entity.get_transform().size; }
    std::vector<Bullet>& getBullets() { return m_bullets; }
    Rectangle getCollisionRect() const;

    const Entity& getShieldEntity() const { return m_shield_entity; }
    const Sprite& getShieldSprite() const { return m_shield_entity.get_sprite(); }
    const transform& getShieldTransform() const { return m_shield_entity.get_transform(); }
    Vector2 getShieldSize() const { return m_shield_entity.get_transform().size; }
    float getShieldStrength() const { return m_shieldStrength; }
    float getMaxShieldStrength() const { return m_maxShieldStrength; }
    void takeDamage(float damage);
    void updateShield();

    void hyperspaceJump(Vector2 newPosition);

    Entity& getEntity() { return m_entity; }
    
private:
    Entity m_entity;
    Vector2 m_velocity = {0, 0};
    Vector2 m_worldSize;
    Rectangle m_cameraFrame;
    
    const float ROTATION_SPEED = 4.0f;
    const float THRUST_FORCE = 0.2f;
    const float DRAG = 0.98f;
    const float MAX_SPEED = 10.0f;

    const float BULLET_SPEED = 12.0f; // Bullet speed
    const float BULLET_MAX_DISTANCE = 3000.0f; // Maximum distance of bullets
    const double FIRE_RATE = 0.2;

    TextureManager* m_textureManager;
    Entity m_bullet_entity;
    Texture2D m_bullet_texture;
    double m_lastShotTime = 0.0;
    std::vector<Bullet> m_bullets;

    // Shield related
    float m_shieldStrength = 0.0f;
    float m_maxShieldStrength = 100.0f;
    float m_shieldRegenRate = 10.0f; // Restore 10 shield points per second
    double m_lastHitTime = 0.0;
    const double SHIELD_REGEN_DELAY = 3.0; // Recovery starts 3 seconds after injury
    Entity m_shield_entity; // Shield Elf Entity

    double m_lastJumpTime = 0.0;
    const double JUMP_COOLDOWN = 1.5;
};