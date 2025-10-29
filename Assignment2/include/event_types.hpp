// event_types.hpp
#pragma once
#include <raylib.h>

enum class EventType {
    Collision,
    PlayerShoot,
    AsteroidDestroyed,
    PlayerHit,
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
    void* dataA; // 指向碰撞实体的数据
    void* dataB;

    CollisionData(EntityType a, EntityType b, Vector2 pos, void* da = nullptr, void* db = nullptr)
        : entityA(a), entityB(b), position(pos), dataA(da), dataB(db) {}
};

/*
struct PlayerHitData {
    Vector2 position;
    int damage;
    int remainingShields; // 如果实现了护盾系统
    int remainingLives;   // 如果实现了生命系统

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