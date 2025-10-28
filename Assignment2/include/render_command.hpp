// render_command.hpp

#pragma once
#include <raylib.h>

enum class RenderCommandType
{
    Star,
    Asteroid,
    Player,
    Bullet
};

struct RenderCommand
{
    RenderCommandType type;
    Vector2 position;
    Vector2 size;
    float rotation;
    Color color;
    int layer;

    Texture2D texture;
};