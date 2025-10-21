// application.cpp

#include "application.hpp"
#include "math_utils.hpp"
#include <raylib.h>
#include <iostream>
#include <algorithm>

bool Application::initialize(int width, int height)
{
    this->m_width = width;
    this->m_height = height;

    m_worldSize = { 10000, 10000 };

    // Initialize World Grid (10x10 sections£¬Each 1000x1000 pixel)
    m_grid.initialize(10, 10, 1000, 1000, m_width, m_height);

    // Generate 6000 asteroids
    m_totalAsteroids = 6000;
    m_grid.generateAsteroids(m_totalAsteroids);

    // Initialize the player's position at the center of the world
    m_player.initialize({ m_worldSize.x / 2.0f, m_worldSize.y / 2.0f });

    // Initialize camera
    Vector2 viewportSize = { (float)m_width, (float)m_height };
    Vector2 screenSize = { (float)m_width, (float)m_height };
    m_camera.initialize(m_player.getPosition(), viewportSize, m_worldSize, screenSize);

    m_player.setViewParameter(m_worldSize, m_camera.getCameraFrame());

    // Initialize starfield
    m_starfield.initialize((int)m_worldSize.x, (int)m_worldSize.y); // Enter world size

    std::cout << "Application initialized successfully" << std::endl;
    return true;
}

void Application::shutdown()
{
    // Clean up resources
}

void Application::update()
{
    processInput();

    // Update players
    m_player.update();

    // Update camera to follow players
    updateCamera();

    // Update asteroid rotation
    m_grid.updateAsteroids();

    // Collect rendering commands
    collectRenderCommands();
}

void Application::processInput()
{
    // Player control
    if (IsKeyDown(KEY_W)) m_player.applyThrust();
    if (IsKeyDown(KEY_A)) m_player.rotateLeft();
    if (IsKeyDown(KEY_D)) m_player.rotateRight();

    // Switch debugging display
    if (IsKeyPressed(KEY_F1)) m_showDebug = !m_showDebug;
}

void Application::updateCamera()
{
    m_camera.update(m_player.getPosition());
}

void Application::collectRenderCommands()
{
    m_renderCommands.clear();
    m_visibleAsteroids = 0;

    // Add starry sky to rendering queue (background layer)
    m_starfield.addRenderCommands(m_renderCommands, m_camera);

    // Obtain asteroids in the visible region
    Rectangle frustum = m_camera.getFrustum();
    auto visibleAsteroids = m_grid.getVisibleAsteroids(frustum);
    m_visibleAsteroids = (int)visibleAsteroids.size();

    // Add asteroid to rendering queue
    for (const auto& asteroid : visibleAsteroids)
    {
        RenderCommand cmd;
        cmd.type = RenderCommandType::Asteroid;
        cmd.position = asteroid.position;
        cmd.rotation = asteroid.rotation;
        cmd.size = asteroid.size;
        cmd.color = asteroid.color;
        cmd.layer = asteroid.layer; // Set hierarchy based on size

        m_renderCommands.push_back(cmd);
    }

    // Add players to the rendering queue (top-level)
    RenderCommand playerCmd;
    playerCmd.type = RenderCommandType::Player;
    playerCmd.position = m_player.getPosition();
    playerCmd.rotation = m_player.getRotation();
    playerCmd.size = { 30, 30 };
    playerCmd.color = RED;
    playerCmd.layer = 10; // The highest level

    m_renderCommands.push_back(playerCmd);

    // Sort rendering commands by hierarchy
    std::sort(m_renderCommands.begin(), m_renderCommands.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            return a.layer < b.layer;
        });
}

void Application::render()
{
    // Obtain camera information
    Vector2 cameraPos = m_camera.getPosition();
    Vector2 viewportSize = m_camera.getViewportSize();
    Rectangle cameraFrame = m_camera.getCameraFrame();

    // Calculate the conversion ratio from world coordinates to camera frame coordinates
    float scaleX = cameraFrame.width / viewportSize.x;
    float scaleY = cameraFrame.height / viewportSize.y;

    // Execute sorted rendering commands
    for (const auto& cmd : m_renderCommands)
    {
        // Convert world coordinates to screen coordinates within the camera frame
        Vector2 screenPos = {
            cameraFrame.x + (cmd.position.x - cameraPos.x + viewportSize.x / 2) * scaleX,
            cameraFrame.y + (cmd.position.y - cameraPos.y + viewportSize.y / 2) * scaleY
        };

        // Adjust the size ratio according to the command type
        Vector2 scaledSize = {
            cmd.size.x * scaleX,
            cmd.size.y * scaleY
        };

        switch (cmd.type)
        {
        case RenderCommandType::Star:
            DrawRectanglePro(
                Rectangle{ screenPos.x, screenPos.y, scaledSize.x, scaledSize.y },
                { scaledSize.x / 2, scaledSize.y / 2 },
                cmd.rotation,
                cmd.color
            );
            break;

        case RenderCommandType::Asteroid:
            DrawRectanglePro(
                Rectangle{ screenPos.x, screenPos.y, scaledSize.x, scaledSize.y },
                { scaledSize.x / 2, scaledSize.y / 2 },
                cmd.rotation,
                cmd.color
            );
            break;

        case RenderCommandType::Player:
            // The player is always in the center of the camera frame
            Vector2 center = {
                cameraFrame.x + cameraFrame.width / 2,
                cameraFrame.y + cameraFrame.height / 2
            };

            Vector2 front = {
                center.x + cosf(cmd.rotation) * scaledSize.x,
                center.y + sinf(cmd.rotation) * scaledSize.y
            };
            Vector2 left = {
                center.x + cosf(cmd.rotation + 2.5f) * scaledSize.x,
                center.y + sinf(cmd.rotation + 2.5f) * scaledSize.y
            };
            Vector2 right = {
                center.x + cosf(cmd.rotation - 2.5f) * scaledSize.x,
                center.y + sinf(cmd.rotation - 2.5f) * scaledSize.y
            };

            DrawTriangle(front, right, left, cmd.color);
            break;
        }
    }

    // Rendering and debugging information
    if (m_showDebug)
    {
        renderDebugInfo();
        m_grid.renderDebug(m_camera);
        m_camera.renderDebug();
    }
}

void Application::renderDebugInfo()
{
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GRAY);
    DrawText(TextFormat("Visible: %d/%d", m_visibleAsteroids, m_totalAsteroids), 10, 35, 20, GRAY);
    DrawText(TextFormat("Position: (%.1f, %.1f)",
        m_player.getPosition().x,
        m_player.getPosition().y), 10, 60, 20, GRAY);

    // Display control prompts
    DrawText("Controls: W - Thrust, A/D - Rotate, F1 - Toggle Debug", 10, m_height - 30, 20, GRAY);
}