// starfield.hpp

#pragma once
#include "render_command.hpp"
#include "game_camera.hpp"
#include <vector>

class Starfield
{
public:
    void initialize(int worldWidth, int worldHeight);
    void addRenderCommands(std::vector<RenderCommand>& commands, const GameCamera& camera);
    
private:
    static const int STAR_COUNT = 1000;
    
    struct Star
    {
        Vector2 position;
        float size;
        Color color;
        float parallaxFactor;
    };
    
    std::vector<Star> m_stars;
    int m_screenWidth = 0;
    int m_screenHeight = 0;
};