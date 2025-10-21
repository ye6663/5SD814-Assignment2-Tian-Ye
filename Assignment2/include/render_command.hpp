// render_command.hpp

#pragma once
#include <raylib.h>

enum class RenderCommandType
{
    Star,
    Asteroid,
    Player
};

struct RenderCommand
{
    RenderCommandType type;
    Vector2 position;
    Vector2 size;
    float rotation;
    Color color;
    int layer;
};