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

    // Initialize World Grid (10x10 sections，Each 1000x1000 pixel)
    m_grid.initialize(10, 10, 1000, 1000, m_width, m_height);

    // Generate 2000 asteroids
    m_totalAsteroids = 2000;
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

    // 初始化事件系统
    m_audioSystem.initialize();
    m_scoreSystem.initialize();
    m_gameplaySystem.initialize();

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

    // Update bullets
    updateBullets();

    // Collect rendering commands
    collectRenderCommands();
}

void Application::processInput()
{
    // Player control
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) m_player.applyThrust();
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) m_player.rotateLeft();
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) m_player.rotateRight();

    // Shoot bullets
    if (IsKeyDown(KEY_SPACE))
    {
        m_player.shoot();
    }

    // Switch debugging display
    if (IsKeyPressed(KEY_F1)) m_showDebug = !m_showDebug;
}

void Application::updateCamera()
{
    m_camera.update(m_player.getPosition());
}

void Application::updateBullets()
{
    auto& bullets = m_player.getBullets();

    // Update bullet position and remove bullets that exceed the distance
    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        if (!it->update()) // If update returns false, it means the bullet should be destroyed
        {
            it = bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Detecting collisions between bullets and stars
    checkBulletCollisions();
}

void Application::checkBulletCollisions()
{
    auto& bullets = m_player.getBullets();
    // const auto& stars = m_starfield.getStars();
    Rectangle frustum = m_camera.getFrustum();
    auto visibleAsteroids = m_grid.getVisibleAsteroids(frustum);

    // Wide phase collision detection: first check which bullets may collide with stars
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
    {
        bool bulletDestroyed = false;
        Rectangle bulletRect = bulletIt->getCollisionRect();

        // Narrow phase collision detection: precise detection of collisions with stars
        for (const auto& asteroid : visibleAsteroids)
        {
            // Create collision rectangles for stars
            Rectangle asteroidRect = {
                asteroid.position.x - asteroid.size.x / 2,
                asteroid.position.y - asteroid.size.y / 2,
                asteroid.size.x,
                asteroid.size.y
            };

            if (CheckCollisionRecs(bulletRect, asteroidRect))
            {
                // 发布碰撞事件而不是直接处理
                CollisionData collisionData(
                    EntityType::Bullet,
                    EntityType::Asteroid,
                    bulletIt->getPosition(),
                    (void*)&(*bulletIt),
                    (void*)&asteroid
                );
                EventSystem::getInstance().publish(EventType::Collision, collisionData);

                bulletDestroyed = true;
                break; // The bullet disappears when it hits a star, there is no need to continue detecting other stars
            }
        }

        if (bulletDestroyed)
        {
            bulletIt = bullets.erase(bulletIt);
        }
        else
        {
            ++bulletIt;
        }
    }

    // 检查玩家与小行星碰撞
    Rectangle playerRect = {
        m_player.getPosition().x - m_player.getSize().x / 2,
        m_player.getPosition().y - m_player.getSize().y / 2,
        m_player.getSize().x,
        m_player.getSize().y
    };

    for (const auto& asteroid : visibleAsteroids) {
        Rectangle asteroidRect = {
            asteroid.position.x - asteroid.size.x / 2,
            asteroid.position.y - asteroid.size.y / 2,
            asteroid.size.x,
            asteroid.size.y
        };

        if (CheckCollisionRecs(playerRect, asteroidRect)) {
            /*
            // 发布玩家被击中事件
            PlayerHitData hitData(
                m_player.getPosition(),
                1, // 伤害值
                0, // 剩余护盾（暂时为0）
                0  // 剩余生命（暂时为0）
            );

            EventSystem::getInstance().publish(EventType::PlayerHit, hitData);
            */

            // 同时发布碰撞事件
            CollisionData collisionData(
                EntityType::Player,
                EntityType::Asteroid,
                m_player.getPosition(),
                (void*)&m_player,
                (void*)&asteroid
            );

            EventSystem::getInstance().publish(EventType::Collision, collisionData);

            break; // 一次只处理一个碰撞
        }
    }

    /*
    // 处理新生成的小行星
    auto newAsteroids = m_gameplaySystem.getPendingAsteroids();
    if (!newAsteroids.empty()) {
    }
    */
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

    // Add bullets to the rendering queue
    for (const auto& bullet : m_player.getBullets())
    {
        RenderCommand cmd;
        cmd.type = RenderCommandType::Bullet;
        cmd.position = bullet.getPosition();
        cmd.rotation = bullet.getRotation(); // Angle calculated using direction
        cmd.size = bullet.getSize();
        cmd.color = WHITE; // The bullet color and texture will cover this
        cmd.layer = 5; // Bullet level
        cmd.texture = bullet.getTexture(); // Bullet texture

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
        {
            // The player is always in the center of the camera frame
            Vector2 center = {
                cameraFrame.x + cameraFrame.width / 2,
                cameraFrame.y + cameraFrame.height / 2
            };

            // Get player texture
            Texture2D playerTexture = m_player.getTexture();
            Vector2 textureSize = m_player.getSize();

            // Draw rotated textures
            Rectangle sourceRec = { 0, 0, (float)playerTexture.width, (float)playerTexture.height };
            Rectangle destRec = {
                center.x,
                center.y,
                textureSize.x,
                textureSize.y
            };
            Vector2 origin = { textureSize.x / 2, textureSize.y / 2 }; // Rotate the origin as the center
            float rotationDegrees = cmd.rotation * RAD2DEG + 90;

            DrawTexturePro(playerTexture, sourceRec, destRec, origin, rotationDegrees, WHITE);
            break;
        }

        case RenderCommandType::Bullet:
        {
            // Draw bullet texture
            Rectangle sourceRec = { 0, 0, (float)cmd.texture.width, (float)cmd.texture.height };
            Rectangle destRec = { screenPos.x, screenPos.y, scaledSize.x, scaledSize.y };
            Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };
            float rotationDegrees = cmd.rotation * RAD2DEG + 90;

            DrawTexturePro(cmd.texture, sourceRec, destRec, origin, rotationDegrees, WHITE);
            break;
        }
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

    DrawText(TextFormat("Score: %d", m_scoreSystem.getScore()), m_width / 2 - 50, 10, 20, RED); // 分数显示

    // Display control prompts
    DrawText("Controls: W/UP - Thrust, A/D/LEFT/RIGHT - Rotate, SPACE - Shoot, F1 - Toggle Debug", 10, m_height - 30, 20, GRAY);
}