// game_camera.hpp

#pragma once
#include <raylib.h>

class GameCamera
{
public:
    void initialize(Vector2 position, Vector2 viewportSize, Vector2 worldSize, Vector2 screenSize);
    void update(Vector2 targetPosition);

    void setPosition(Vector2 position) { m_position = position; }
    Vector2 getPosition() const { return m_position; }

    Rectangle getFrustum() const { return m_frustum; }
    Vector2 getViewportSize() const { return m_viewportSize; }

    // Get the position and size of the camera frame on the screen
    Rectangle getCameraFrame() const { return m_cameraFrame; }

    void renderDebug() const;

private:
    Vector2 m_position = { 0, 0 };
    Vector2 m_viewportSize = { 0, 0 };    // Camera viewport size (in world units)
    Vector2 m_screenSize = { 0, 0 };      // Screen size
    Vector2 m_worldSize = { 0, 0 };
    Rectangle m_frustum = { 0, 0, 0, 0 };

    // The position and size of the camera frame on the screen
    Rectangle m_cameraFrame = { 0, 0, 0, 0 };

    void clampToWorldBounds();
    void updateCameraFrame(Vector2 targetPosition); // Update camera frame position
};