// render_command.hpp

#pragma once
#include "sprite.hpp"
#include "transform.hpp"
#include <raylib.h>

enum class RenderCommandType
{
    Star,
    Sprite
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
    const Sprite* sprite = nullptr;
    const transform* transform = nullptr;
};