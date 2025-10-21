// starfield.cpp

#include "starfield.hpp"
#include "math_utils.hpp"
#include <raylib.h>

void Starfield::initialize(int worldWidth, int worldHeight)
{
    m_screenWidth = worldWidth;
    m_screenHeight = worldHeight;

    m_stars.resize(STAR_COUNT);

    for (auto& star : m_stars)
    {
        // Random world position
        star.position = {
            MathUtils::random(0.0f, (float)worldWidth),   // 0 to world width
            MathUtils::random(0.0f, (float)worldHeight)   // 0 to world height
        };

        // Random size and color
        star.size = MathUtils::random(1.0f, 3.0f);

        unsigned char brightness = (unsigned char)MathUtils::random(100, 255);
        star.color = { brightness, brightness, brightness, 255 };

        // Random parallax factor (0.1 - 0.9)
        star.parallaxFactor = MathUtils::random(0.1f, 0.9f);
    }
}

void Starfield::addRenderCommands(std::vector<RenderCommand>& commands, const GameCamera& camera)
{
    Rectangle frustum = camera.getFrustum();

    for (const auto& star : m_stars)
    {
        // Check if star is within the camera frustum
        if (star.position.x >= frustum.x && star.position.x <= frustum.x + frustum.width &&
            star.position.y >= frustum.y && star.position.y <= frustum.y + frustum.height)
        {
            RenderCommand cmd;
            cmd.type = RenderCommandType::Star;
            cmd.position = star.position; // Use world coordinates
            cmd.size = { star.size, star.size };
            cmd.rotation = 0;
            cmd.color = star.color;
            cmd.layer = 0;

            commands.push_back(cmd);
        }
    }
}