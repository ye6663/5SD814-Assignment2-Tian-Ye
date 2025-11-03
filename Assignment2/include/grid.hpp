// grid.hpp

#pragma once
#include "asteroid.hpp"
#include "game_camera.hpp"
#include <vector>
#include <any>
#include <raylib.h>

struct GridCell
{
    std::vector<Asteroid> asteroids;
};

class Grid
{
public:
    void initialize(int width, int height, int cellWidth, int cellHeight, int screen_width, int screen_height, TextureManager& textureManager, Vector2 worldSize);
    void generateAsteroids(int count);
    void updateAsteroids();
    
    std::vector<const Asteroid*> getVisibleAsteroids(const Rectangle& frustum) const;
    void splitAsteroid(Asteroid* asteroid);
    void removeMarkedAsteroids();
    void onCollision(const std::any& data);
    void reset();
    void clear();

    void renderDebug(const GameCamera& camera) const;
    
private:
    Vector2 m_worldSize;
    int m_width = 0;
    int m_height = 0;
    int m_cellWidth = 0;
    int m_cellHeight = 0;
    int m_screen_width;
    int m_screen_height;
    
    std::vector<GridCell> m_cells;
    
    // Convert world coordinates to grid coordinates
    void worldToGrid(const Vector2& position, int& gridX, int& gridY) const;
    
    // Check if the grid cells are inside the cone of sight
    bool isCellVisible(int gridX, int gridY, const Rectangle& frustum) const;

    TextureManager* m_textureManager;
    Entity m_asteroid1_entity;
    Entity m_asteroid2_entity;

    void moveAsteroidToNewCell(Asteroid* asteroid, const Vector2& oldPosition, const Vector2& newPosition);
};