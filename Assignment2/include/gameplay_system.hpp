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
        EventSystem::getInstance().subscribe(EventType::Collision,
            [this](const std::any& data) { onCollision(data); });

        std::cout << "GameplaySystem initialized" << std::endl;
    }

    // 用于生成新的小行星
    void spawnAsteroids(const std::vector<Asteroid>& newAsteroids) {
        m_pendingAsteroids.insert(m_pendingAsteroids.end(),
            newAsteroids.begin(), newAsteroids.end());
    }

    std::vector<Asteroid> getPendingAsteroids() {
        std::vector<Asteroid> result = m_pendingAsteroids;
        m_pendingAsteroids.clear();
        return result;
    }

private:
    void onCollision(const std::any& data) {
        try {
            auto collisionData = std::any_cast<CollisionData>(data);

            // 处理子弹与小行星碰撞
            if ((collisionData.entityA == EntityType::Bullet &&
                collisionData.entityB == EntityType::Asteroid) ||
                (collisionData.entityA == EntityType::Asteroid &&
                    collisionData.entityB == EntityType::Bullet)) {

                handleBulletAsteroidCollision(collisionData);
            }
            // 处理玩家与小行星碰撞
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

        // 获取小行星指针
        if (collisionData.entityA == EntityType::Asteroid) {
            asteroid = static_cast<Asteroid*>(collisionData.dataA);
        }
        else {
            asteroid = static_cast<Asteroid*>(collisionData.dataB);
        }

        if (asteroid) {
            // 根据小行星大小确定分数和分裂逻辑
            int asteroidSize = determineAsteroidSize(*asteroid);
            int scoreValue = calculateScore(asteroidSize);

            // 发布小行星被摧毁事件
            EventSystem::getInstance().publish(EventType::AsteroidDestroyed,
                AsteroidDestroyedData(asteroid->position, asteroidSize, scoreValue));

            // 生成更小的小行星
            spawnSmallerAsteroids(*asteroid, asteroidSize);
        }
    }

    void handlePlayerAsteroidCollision(const CollisionData& collisionData) {
        // std::cout << "Gameplay: Player hit asteroid!" << std::endl;
        EventSystem::getInstance().publish(EventType::PlayerHit);
        // 可以在这里处理玩家生命值减少等逻辑
    }

    int determineAsteroidSize(const Asteroid& asteroid) {
        // 根据大小判断小行星类型
        float avgSize = (asteroid.size.x + asteroid.size.y) / 2.0f;
        if (avgSize < 30.0f) return 1; // 小
        else if (avgSize < 50.0f) return 2; // 中
        else return 3; // 大
    }

    int calculateScore(int asteroidSize) {
        switch (asteroidSize) {
        case 1: return 100;  // 小行星
        case 2: return 50;   // 中行星
        case 3: return 20;   // 大行星
        default: return 0;
        }
    }

    void spawnSmallerAsteroids(const Asteroid& original, int originalSize) {
        if (originalSize > 1) {
            int spawnCount = (originalSize == 3) ? 2 : 3; // 大行星分裂成2个中行星，中行星分裂成3个小行星

            for (int i = 0; i < spawnCount; i++) {
                Asteroid newAsteroid;
                Vector2 newSize = {
                    original.size.x * 0.6f,
                    original.size.y * 0.6f
                };

                // 在原始位置附近生成
                Vector2 newPos = {
                    original.position.x + (float)(rand() % 60 - 30),
                    original.position.y + (float)(rand() % 60 - 30)
                };

                newAsteroid.initialize(newPos, newSize, original.rotation,
                    original.rotationSpeed, original.color, original.layer);

                m_pendingAsteroids.push_back(newAsteroid);
            }
        }
    }

    /*
    void playerHit() {
        try {
            if (m_playerShields > 0) {
                // 先扣除护盾
                m_playerShields -=  10; // 假设1点伤害扣除10点护盾
                if (m_playerShields < 0) m_playerShields = 0;

                std::cout << "Player shield hit! Remaining: " << m_playerShields << std::endl;
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
    */

    std::vector<Asteroid> m_pendingAsteroids;

    int m_playerLives;
    int m_playerShields;
    bool m_isGameOver;
};