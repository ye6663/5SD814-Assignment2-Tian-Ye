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
        try {
            double currentTime = GetTime();
            if (!m_canTakeDamage) {
                if (currentTime - m_lastHitTime >= HIT_COOLDOWN) {
                    m_canTakeDamage = true;
                }
                else {
                    return;
                }
            }

            Player* player = nullptr;
            Asteroid* asteroid = nullptr;
            if (collisionData.entityA == EntityType::Player) {
                player = static_cast<Player*>(collisionData.dataA);
                asteroid = static_cast<Asteroid*>(collisionData.dataB);
            }
            else {
                player = static_cast<Player*>(collisionData.dataB);
                asteroid = static_cast<Asteroid*>(collisionData.dataA);
            }

            if (player && asteroid) {
                if (player->getShieldStrength() > 0) {
                    int asteroidSize = determineAsteroidSize(*asteroid);
                    int scoreValue = calculateScore(asteroidSize);
                    EventSystem::getInstance().publish(EventType::AsteroidDestroyed, AsteroidDestroyedData(asteroid->getEntity().get_transform().position, asteroidSize, scoreValue));
                }
                EventSystem::getInstance().publish(EventType::PlayerHit, 20.0f);
            }

            // EventSystem::getInstance().publish(EventType::PlayerHit, 20.0f);
            m_canTakeDamage = false;
            m_lastHitTime = currentTime;
        }
        catch (const std::bad_any_cast&) {
            std::cout << "GameplaySystem: Invalid player hit data" << std::endl;
        }
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

    double m_lastHitTime = 0.0;
    const double HIT_COOLDOWN = 1.0;
    bool m_canTakeDamage = true;
};