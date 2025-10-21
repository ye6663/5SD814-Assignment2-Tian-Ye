// game_camera.cpp

#include "game_camera.hpp"
#include <raylib.h>
#include <algorithm>

void GameCamera::initialize(Vector2 position, Vector2 viewportSize, Vector2 worldSize, Vector2 screenSize)
{
    m_position = position;
    m_viewportSize = viewportSize;
    m_worldSize = worldSize;
    m_screenSize = screenSize;

    // Initialize the camera frame to the center of the screen, with half the viewport size
    m_cameraFrame.width = m_viewportSize.x / 2;
    m_cameraFrame.height = m_viewportSize.y / 2;
    m_cameraFrame.x = (m_screenSize.x - m_cameraFrame.width) / 2;
    m_cameraFrame.y = (m_screenSize.y - m_cameraFrame.height) / 2;

    // Calculate the initial frustum (centered around the camera)
    m_frustum.x = m_position.x - m_viewportSize.x / 2;
    m_frustum.y = m_position.y - m_viewportSize.y / 2;
    m_frustum.width = m_viewportSize.x;
    m_frustum.height = m_viewportSize.y;

    clampToWorldBounds();
}

void GameCamera::update(Vector2 targetPosition)
{
    // Update the camera frame position first
    updateCameraFrame(targetPosition);

    // Then update the world camera position based on the camera frame's position
    // Calculate the offset of the target position relative to the camera frame's center
    float offsetX = (targetPosition.x - m_position.x) * (m_viewportSize.x / m_cameraFrame.width);
    float offsetY = (targetPosition.y - m_position.y) * (m_viewportSize.y / m_cameraFrame.height);

    m_position.x += offsetX;
    m_position.y += offsetY;

    // Clamp the camera to stay within the world bounds
    clampToWorldBounds();

    // Update the frustum
    m_frustum.x = m_position.x - m_viewportSize.x / 2;
    m_frustum.y = m_position.y - m_viewportSize.y / 2;
    m_frustum.width = m_viewportSize.x;
    m_frustum.height = m_viewportSize.y;
}

void GameCamera::updateCameraFrame(Vector2 targetPosition)
{
    // Calculate the player's screen coordinates relative to the world camera
    float screenX = m_cameraFrame.x + m_cameraFrame.width / 2;
    float screenY = m_cameraFrame.y + m_cameraFrame.height / 2;

    // Calculate the direction and distance the player should move (relative to the camera frame's center)
    float deltaX = (targetPosition.x - m_position.x) * (m_cameraFrame.width / m_viewportSize.x);
    float deltaY = (targetPosition.y - m_position.y) * (m_cameraFrame.height / m_viewportSize.y);

    // Move the camera frame
    m_cameraFrame.x += deltaX;
    m_cameraFrame.y += deltaY;

    // Clamp the camera frame to stay within the screen bounds
    if (m_cameraFrame.x < 0) m_cameraFrame.x = 0;
    if (m_cameraFrame.y < 0) m_cameraFrame.y = 0;
    if (m_cameraFrame.x + m_cameraFrame.width > m_screenSize.x)
        m_cameraFrame.x = m_screenSize.x - m_cameraFrame.width;
    if (m_cameraFrame.y + m_cameraFrame.height > m_screenSize.y)
        m_cameraFrame.y = m_screenSize.y - m_cameraFrame.height;
}

void GameCamera::clampToWorldBounds()
{
    // Ensure the camera does not exceed the left world boundary
    if (m_position.x - m_viewportSize.x / 2 < 0) {
        m_position.x = m_viewportSize.x / 2;
    }
    // Ensure the camera does not exceed the right world boundary
    else if (m_position.x + m_viewportSize.x / 2 > m_worldSize.x) {
        m_position.x = m_worldSize.x - m_viewportSize.x / 2;
    }

    // Ensure the camera does not exceed the top world boundary
    if (m_position.y - m_viewportSize.y / 2 < 0) {
        m_position.y = m_viewportSize.y / 2;
    }
    // Ensure the camera does not exceed the bottom world boundary
    else if (m_position.y + m_viewportSize.y / 2 > m_worldSize.y) {
        m_position.y = m_worldSize.y - m_viewportSize.y / 2;
    }
}

void GameCamera::renderDebug() const
{
    // Draw the camera frame on the screen
    DrawRectangleLinesEx(m_cameraFrame, 1.0f, GREEN);

    // Draw the camera's center point
    DrawCircle((int)(m_cameraFrame.x + m_cameraFrame.width / 2), (int)(m_cameraFrame.y + m_cameraFrame.height / 2), 2.0f, GREEN);

    // Display camera information
    DrawText(TextFormat("Camera Frame: (%.0f, %.0f)", m_cameraFrame.x, m_cameraFrame.y),
        (int)m_cameraFrame.x + 0, (int)m_cameraFrame.y - 13, 11, GREEN);
    DrawText(TextFormat("Frustum Size: %.0fx%.0f", m_cameraFrame.width, m_cameraFrame.height),
        (int)m_cameraFrame.x + 160, (int)m_cameraFrame.y - 13, 11, GREEN);
    DrawText(TextFormat("World Camera: (%.0f, %.0f)", m_position.x, m_position.y),
        (int)m_cameraFrame.x + 300, (int)m_cameraFrame.y - 13, 11, GREEN);
}