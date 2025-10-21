// grid.cpp

#include "grid.hpp"
#include "game_camera.hpp"
#include "math_utils.hpp"
#include <raylib.h>
#include <iostream>

void Grid::initialize(int width, int height, int cellWidth, int cellHeight, int screen_width, int screen_height)
{
    m_width = width;
    m_height = height;
    m_cellWidth = cellWidth;
    m_cellHeight = cellHeight;
    m_screen_width = screen_width;
    m_screen_height = screen_height;

    m_cells.resize(width * height);

    std::cout << "Grid initialized: " << width << "x" << height
        << " (" << width * height << " cells)" << std::endl;
}

void Grid::generateAsteroids(int count)
{
    for (int i = 0; i < count; i++)
    {
        Asteroid asteroid;

        // Random position within world bounds
        Vector2 position = {
            (float)MathUtils::random(0, m_width * m_cellWidth),
            (float)MathUtils::random(0, m_height * m_cellHeight)
        };

        // Random size
        float sizeType = (float)MathUtils::random(0, 3);
        Vector2 size;
        int layer;

        if (sizeType < 1.0f) {
            size = { 20, 20 };
            layer = 1; // Background layer
        }
        else if (sizeType < 2.0f) {
            size = { 40, 40 };
            layer = 2; // Middle layer
        }
        else {
            size = { 60, 60 };
            layer = 3; // Foreground layer
        }

        // Random rotation and rotation direction
        float rotation = (float)MathUtils::random(0, 360);
        float rotationSpeed = MathUtils::random(0.2f, 1.0f);
        if (MathUtils::random(0, 2) < 1.0f) {
            rotationSpeed = -rotationSpeed; // Random clockwise or counter-clockwise
        }

        // Random grayscale color
        unsigned char gray = (unsigned char)MathUtils::random(150, 230);
        Color color = { gray, gray, gray, 255 };

        asteroid.initialize(position, size, rotation, rotationSpeed, color, layer);

        // Add to corresponding grid cell
        int gridX, gridY;
        worldToGrid(position, gridX, gridY);

        if (gridX >= 0 && gridX < m_width && gridY >= 0 && gridY < m_height)
        {
            int index = gridY * m_width + gridX;
            m_cells[index].asteroids.push_back(asteroid);
        }
    }

    std::cout << "Generated " << count << " asteroids" << std::endl;
}

void Grid::updateAsteroids()
{
    for (auto& cell : m_cells)
    {
        for (auto& asteroid : cell.asteroids)
        {
            asteroid.update();
        }
    }
}

std::vector<Asteroid> Grid::getVisibleAsteroids(const Rectangle& frustum) const
{
    std::vector<Asteroid> result;

    // Calculate grid range covered by the frustum
    int startX = std::max(0, static_cast<int>(frustum.x) / m_cellWidth);
    int endX = std::min(m_width - 1, static_cast<int>(frustum.x + frustum.width) / m_cellWidth);

    int startY = std::max(0, static_cast<int>(frustum.y) / m_cellHeight);
    int endY = std::min(m_height - 1, static_cast<int>(frustum.y + frustum.height) / m_cellHeight);

    // Iterate through visible grid cells
    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            int index = y * m_width + x;
            const auto& cell = m_cells[index];

            // Check if each asteroid is within the frustum
            for (const auto& asteroid : cell.asteroids)
            {
                Rectangle asteroidRect = {
                    asteroid.position.x - asteroid.size.x / 2,
                    asteroid.position.y - asteroid.size.y / 2,
                    asteroid.size.x,
                    asteroid.size.y
                };

                if (CheckCollisionRecs(asteroidRect, frustum))
                {
                    result.push_back(asteroid);
                }
            }
        }
    }

    return result;
}

void Grid::renderDebug(const GameCamera& camera) const
{
    Rectangle frustum = camera.getFrustum();
    Rectangle cameraFrame = camera.getCameraFrame();

    // Draw grid lines
    Vector2 cameraPos = camera.getPosition();
    Vector2 viewportSize = camera.getViewportSize();
    // Calculate conversion ratio from world coordinates to camera frame coordinates
    float scaleX = cameraFrame.width / viewportSize.x;
    float scaleY = cameraFrame.height / viewportSize.y;
    for (int x = 0; x <= m_width; x++)
    {
        int worldX = x * m_cellWidth;

        // Convert world coordinates to screen coordinates
        int screenX1 = (int)(cameraFrame.x + (worldX - cameraPos.x + viewportSize.x / 2) * scaleX);
        int screenY1 = (int)cameraFrame.y;
        int screenX2 = screenX1;
        int screenY2 = (int)(cameraFrame.y + cameraFrame.height);

        // Only draw grid lines within the camera frame
        if (screenX1 >= cameraFrame.x && screenX1 <= cameraFrame.x + cameraFrame.width)
        {
            DrawLine(screenX1, screenY1, screenX2, screenY2, Fade(DARKGRAY, 0.5f));
        }
    }

    for (int y = 0; y <= m_height; y++)
    {
        int worldY = y * m_cellHeight;

        // Convert world coordinates to screen coordinates
        int screenX1 = (int)cameraFrame.x;
        int screenY1 = (int)(cameraFrame.y + (worldY - cameraPos.y + viewportSize.y / 2) * scaleY);
        int screenX2 = (int)(cameraFrame.x + cameraFrame.width);
        int screenY2 = screenY1;

        // Only draw grid lines within the camera frame
        if (screenY1 >= cameraFrame.y && screenY1 <= cameraFrame.y + cameraFrame.height)
        {
            DrawLine(screenX1, screenY1, screenX2, screenY2, Fade(DARKGRAY, 0.5f));
        }
    }

    // Draw minimap
    int miniMapSize = m_screen_width / 10;
    int miniMapX = m_screen_width - miniMapSize;
    int miniMapY = 0;
    DrawRectangle(miniMapX, miniMapY, miniMapSize, miniMapSize, Fade(BLACK, 0.5f));

    // Calculate scaling factor
    float scale = static_cast<float>(miniMapSize) / (m_width * m_cellWidth);

    // Draw grid
    for (int x = 0; x <= m_width; x++)
    {
        int lineX = miniMapX + (int)(x * m_cellWidth * scale);
        DrawLine(lineX, miniMapY, lineX, miniMapY + miniMapSize, DARKGRAY);
    }

    for (int y = 0; y <= m_height; y++)
    {
        int lineY = miniMapY + (int)(y * m_cellHeight * scale);
        DrawLine(miniMapX, lineY, miniMapX + miniMapSize, lineY, DARKGRAY);
    }

    // Draw frustum on minimap
    Rectangle miniFrustum = {
        miniMapX + frustum.x * scale,
        miniMapY + frustum.y * scale,
        frustum.width * scale,
        frustum.height * scale
    };
    DrawRectangleLinesEx(miniFrustum, 1, GREEN);
}

void Grid::worldToGrid(const Vector2& position, int& gridX, int& gridY) const
{
    gridX = static_cast<int>(position.x) / m_cellWidth;
    gridY = static_cast<int>(position.y) / m_cellHeight;
}

bool Grid::isCellVisible(int gridX, int gridY, const Rectangle& frustum) const
{
    Rectangle cellRect = {
        static_cast<float>(gridX * m_cellWidth),
        static_cast<float>(gridY * m_cellHeight),
        static_cast<float>(m_cellWidth),
        static_cast<float>(m_cellHeight)
    };

    return CheckCollisionRecs(cellRect, frustum);
}