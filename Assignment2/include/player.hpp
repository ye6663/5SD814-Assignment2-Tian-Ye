// player.hpp

#pragma once
#include <raylib.h>

class Player
{
public:
    void initialize(Vector2 position);
    void setViewParameter(Vector2 worldSize, Rectangle cameraFrame);
    void update();
    
    void applyThrust();
    void rotateLeft();
    void rotateRight();
    
    Vector2 getPosition() const { return m_position; }
    float getRotation() const { return m_rotation; }
    
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
};