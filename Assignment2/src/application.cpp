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

    if (!m_textureManager.load_manifest("assets/manifest.txt")) {
        return false;
    }

    m_worldSize = { 10000, 10000 };

    // Initialize World Grid (10x10 sections£¬Each 1000x1000 pixel)
    m_grid.initialize(10, 10, 1000, 1000, m_width, m_height);

    // Generate 2000 asteroids
    m_totalAsteroids = 2000;
    m_grid.generateAsteroids(m_totalAsteroids);

    // Initialize the player's position at the center of the world
    m_player.initialize({ m_worldSize.x / 2.0f, m_worldSize.y / 2.0f }, m_textureManager);

    // Initialize camera
    Vector2 viewportSize = { (float)m_width, (float)m_height };
    Vector2 screenSize = { (float)m_width, (float)m_height };
    m_camera.initialize(m_player.getPosition(), viewportSize, m_worldSize, screenSize);

    m_player.setViewParameter(m_worldSize, m_camera.getCameraFrame());

    // Initialize starfield
    m_starfield.initialize((int)m_worldSize.x, (int)m_worldSize.y); // Enter world size

    // Initialize event system
    m_audioSystem.initialize();
    m_scoreSystem.initialize();
    m_gameplaySystem.initialize();

    // Initialize main menu
    m_mainMenu.initialize();
    m_mainMenu.setStartCallback([this]() { startGame(); });
    m_mainMenu.setExitCallback([this]() { exitGame(); });

    // Initialize game end screen
    m_gameOverScreen.initialize();
    m_gameOverScreen.setReturnToMenuCallback([this]() { returnToMainMenu(); });
    m_gameOverScreen.setRestartCallback([this]() { restartGame(); });

    // Subscription game end event
    EventSystem::getInstance().subscribe(EventType::GameOver,
        [this](const std::any& data) { showGameOverScreen(); });

    std::cout << "Application initialized successfully" << std::endl;
    return true;
}

void Application::shutdown()
{
    // Clean up resources
    m_textureManager.release_textures();
}

void Application::update()
{
    switch (m_currentState) {
        case GameState::MainMenu:
            m_mainMenu.update();
            break;
        case GameState::Playing:
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
            break;
        case GameState::GameOver:
            m_gameOverScreen.update();
            break;
    }
}

void Application::processInput()
{
    if (m_currentState != GameState::Playing) return;

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
                // Publish collision events
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

    // Check for collisions between players and asteroids
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
            // Publish player hit event
            PlayerHitData hitData(
                m_player.getPosition(),
                1, // Damage value
                0, // Remaining Shield (temporarily 0)
                0  // Remaining life (temporarily 0)
            );

            EventSystem::getInstance().publish(EventType::PlayerHit, hitData);
            */

            // Publish collision events
            CollisionData collisionData(
                EntityType::Player,
                EntityType::Asteroid,
                m_player.getPosition(),
                (void*)&m_player,
                (void*)&asteroid
            );

            EventSystem::getInstance().publish(EventType::Collision, collisionData);

            break; // Only handle one collision at a time
        }
    }

    /*
    // Processing newly generated asteroids
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
        cmd.type = RenderCommandType::Sprite;
        cmd.layer = bullet.getEntity().get_layer(); // Bullet level
        cmd.sprite = &bullet.getEntity().get_sprite();
        cmd.transform = &bullet.getEntity().get_transform();
        cmd.rotation = bullet.getEntity().get_transform().rotation; // Angle calculated using direction
        cmd.size = bullet.getEntity().get_transform().size;
        m_renderCommands.push_back(cmd);
    }

    // Add players to the rendering queue (top-level)
    RenderCommand spriteCmd;
    spriteCmd.type = RenderCommandType::Sprite;
    spriteCmd.layer = m_player.getEntity().get_layer();
    spriteCmd.sprite = &m_player.getEntity().get_sprite();
    spriteCmd.transform = &m_player.getEntity().get_transform();
    spriteCmd.rotation = m_player.getEntity().get_transform().rotation;
    spriteCmd.size = m_player.getEntity().get_transform().size;
    m_renderCommands.push_back(spriteCmd);

    // Sort rendering commands by hierarchy
    std::sort(m_renderCommands.begin(), m_renderCommands.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            return a.layer < b.layer;
        });
}

void Application::renderGame()
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

        case RenderCommandType::Sprite:
            if (cmd.sprite && cmd.transform) {
                TextureHandle textureHandle = cmd.sprite->get_texture();
                Texture2D* texture = m_textureManager.get_texture_from_handle(textureHandle);
                if (texture && texture->id != 0) {
                    Vector2 screenPos2;
                    if (&m_player.getEntity().get_transform() == cmd.transform) {
                        // Player
                        screenPos2 = {
                            cameraFrame.x + cameraFrame.width / 2,
                            cameraFrame.y + cameraFrame.height / 2
                        };
                    }
                    else {
                        // Other entity
                        screenPos2 = {
                            cameraFrame.x + (cmd.transform->position.x - cameraPos.x + viewportSize.x / 2) * scaleX,
                            cameraFrame.y + (cmd.transform->position.y - cameraPos.y + viewportSize.y / 2) * scaleY
                        };
                    }

                    Vector2 scaledSize2 = {
                        cmd.size.x * scaleX,
                        cmd.size.y * scaleY
                    };

                    // Draw sprite
                    Rectangle sourceRec = { 0, 0, (float)texture->width, (float)texture->height };
                    Rectangle destRec = {
                        screenPos2.x,
                        screenPos2.y,
                        scaledSize2.x,
                        scaledSize2.y
                    };
                    Vector2 origin = { scaledSize2.x / 2, scaledSize2.y / 2 };
                    float rotationDegrees = cmd.transform->rotation * RAD2DEG + 90;
                    DrawTexturePro(*texture, sourceRec, destRec, origin, rotationDegrees, cmd.sprite->get_tint());
                }
            }
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

void Application::render()
{
    switch (m_currentState) {
        case GameState::MainMenu:
            m_mainMenu.render();
            break;
        case GameState::Playing:
            renderGame();
            break;
        case GameState::GameOver:
            renderGame();
            m_gameOverScreen.render();
            break;
    }
}

void Application::renderDebugInfo()
{
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GRAY);
    DrawText(TextFormat("Visible: %d/%d", m_visibleAsteroids, m_totalAsteroids), 10, 35, 20, GRAY);
    DrawText(TextFormat("Position: (%.1f, %.1f)",
        m_player.getPosition().x,
        m_player.getPosition().y), 10, 60, 20, GRAY);

    DrawText(TextFormat("Score: %d", m_scoreSystem.getScore()), m_width / 2 - 50, 10, 20, RED); // Score display

    // Display control prompts
    DrawText("Controls: W/UP - Thrust, A/D/LEFT/RIGHT - Rotate, SPACE - Shoot, F1 - Toggle Debug", 10, m_height - 30, 20, GRAY);
}

void Application::startGame() {
    m_currentState = GameState::Playing;
    m_scoreSystem.reset();

    // Reset player position
    m_player.initialize({ m_worldSize.x / 2.0f, m_worldSize.y / 2.0f }, m_textureManager);
    m_camera.setPosition(m_player.getPosition());

    EventSystem::getInstance().publish(EventType::GameStart);
}

void Application::restartGame() {
    startGame();
}

void Application::returnToMainMenu() {
    m_currentState = GameState::MainMenu;
}

void Application::showGameOverScreen() {
    m_currentState = GameState::GameOver;
    m_gameOverScreen.setFinalScore(m_scoreSystem.getScore());
}

void Application::exitGame() {
    CloseWindow();
}