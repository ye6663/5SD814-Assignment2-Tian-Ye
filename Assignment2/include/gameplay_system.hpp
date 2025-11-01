// gameplay_system.hpp
#pragma once
#include "event_system.hpp"
#include "event_types.hpp"
#include "asteroid.hpp"
#include <vector>
#include <iostream>

class GameplaySystem {
public:
    void initialize() {
        EventSystem::getInstance().subscribe(EventType::GameStart,
            [this](const std::any& data) { onGameStart(); });

        EventSystem::getInstance().subscribe(EventType::Collision,
            [this](const std::any& data) { onCollision(data); });

        std::cout << "GameplaySystem initialized" << std::endl;
    }

private:
    void onGameStart() {
        m_playerLives = 10;
        m_playerShields = 0;
        m_isGameOver = false;
    }

    void onCollision(const std::any& data) {
        try {
            auto collisionData = std::any_cast<CollisionData>(data);

            // Dealing with collisions between bullets and asteroids
            if ((collisionData.entityA == EntityType::Bullet &&
                collisionData.entityB == EntityType::Asteroid) ||
                (collisionData.entityA == EntityType::Asteroid &&
                    collisionData.entityB == EntityType::Bullet)) {

                handleBulletAsteroidCollision(collisionData);
            }
            // Dealing with collisions between players and asteroids
            else if ((collisionData.entityA == EntityType::Player &&
                collisionData.entityB == EntityType::Asteroid) ||
                (collisionData.entityA == EntityType::Asteroid &&
                    collisionData.entityB == EntityType::Player)) {

                handlePlayerAsteroidCollision(collisionData);
            }
        }
        catch (const std::bad_any_cast&) {
            std::cout << "GameplaySystem: Invalid collision data" << std::endl;
        }
    }

    void handleBulletAsteroidCollision(const CollisionData& collisionData) {
        Asteroid* asteroid = nullptr;

        // Get asteroid pointer
        if (collisionData.entityA == EntityType::Asteroid) {
            asteroid = static_cast<Asteroid*>(collisionData.dataA);
        }
        else {
            asteroid = static_cast<Asteroid*>(collisionData.dataB);
        }

        if (asteroid) {
            // Determine scores and splitting logic based on asteroid size
            int asteroidSize = determineAsteroidSize(*asteroid);
            int scoreValue = calculateScore(asteroidSize);

            // Publish asteroid destruction event
            EventSystem::getInstance().publish(EventType::AsteroidDestroyed,
                AsteroidDestroyedData(asteroid->getEntity().get_transform().position, asteroidSize, scoreValue));
        }
    }

    void handlePlayerAsteroidCollision(const CollisionData& collisionData) {
        // std::cout << "Gameplay: Player hit asteroid!" << std::endl;
        if (m_isGameOver) {
            return;
        }

        EventSystem::getInstance().publish(EventType::PlayerHit);
        playerHit();
    }

    int determineAsteroidSize(const Asteroid& asteroid) {
        // Determine the type of asteroid based on its size
        Vector2 size = asteroid.getEntity().get_transform().size;
        float avgSize = (size.x + size.y) / 2.0f;
        if (avgSize > 50.0f) return 3; // big
        else if (avgSize > 30.0f) return 2; // medium 
        else return 1; // small
    }

    int calculateScore(int asteroidSize) {
        switch (asteroidSize) {
        case 1: return 2;   // small
        case 2: return 5;   // medium
        case 3: return 10;  // big
        default: return 0;
        }
    }

    void playerHit() {
        try {
            if (m_isGameOver) {
                return;
            }
            if (m_playerShields > 0) {
                // 先扣除护盾
                m_playerShields -=  10; // 假设1点伤害扣除10点护盾
                if (m_playerShields < 0) m_playerShields = 0;

                // std::cout << "Player shield hit! Remaining: " << m_playerShields << std::endl;
            }
            else {
                // 护盾为0时扣除生命
                m_playerLives -= 10;
                std::cout << "Player hit! Lives remaining: " << m_playerLives << std::endl;

                if (m_playerLives <= 0) {
                    m_playerLives = 0;
                    m_isGameOver = true;
                    std::cout << "Game Over!" << std::endl;

                    // 发布游戏结束事件
                    EventSystem::getInstance().publish(EventType::GameOver);
                }
            }
        }
        catch (const std::bad_any_cast&) {
            std::cout << "GameplaySystem: Invalid player hit data" << std::endl;
        }
    }

    int m_playerLives = 10;
    int m_playerShields = 0;
    bool m_isGameOver = false;
};