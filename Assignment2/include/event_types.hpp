// event_types.hpp

#pragma once
#include <raylib.h>

enum class EventType {
    Collision,
    PlayerShoot,
    AsteroidDestroyed,
    PlayerHit,
    CollisionAsteroid,
    HyperspaceJump,
    GameStart,
    GameOver,
    ScoreChanged
};

enum class EntityType {
    Player,
    Bullet,
    Asteroid,
    Unknown
};

struct CollisionData {
    EntityType entityA;
    EntityType entityB;
    Vector2 position;
    void* dataA; // Data pointing to collision entities
    void* dataB;

    CollisionData(EntityType a, EntityType b, Vector2 pos, void* da = nullptr, void* db = nullptr)
        : entityA(a), entityB(b), position(pos), dataA(da), dataB(db) {}
};

/*
struct PlayerHitData {
    Vector2 position;
    int damage;
    int remainingShields;
    int remainingLives;

    PlayerHitData(Vector2 pos, int dmg = 1, int shields = 0, int lives = 0)
        : position(pos), damage(dmg), remainingShields(shields), remainingLives(lives) {}
};
*/

struct AsteroidDestroyedData {
    Vector2 position;
    int asteroidSize; // 1=small, 2=medium, 3=large
    int scoreValue;

    AsteroidDestroyedData(Vector2 pos, int size, int score)
        : position(pos), asteroidSize(size), scoreValue(score) {}
};

struct ScoreChangedData {
    int newScore;
    int scoreChange;

    ScoreChangedData(int score, int change = 0)
        : newScore(score), scoreChange(change) {}
};