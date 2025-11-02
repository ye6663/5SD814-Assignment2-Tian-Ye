// sprite.hpp

#pragma once
#include "texture_manager.hpp"
#include <raylib.h>

class Sprite {
public:
    Sprite();
    Sprite(TextureHandle texture);

    void set_texture(TextureHandle texture);
    void set_tint(Color tint);

    TextureHandle get_texture() const { return m_texture; }
    Color get_tint() const { return m_tint; }

private:
    TextureHandle m_texture;
    Color m_tint = WHITE;
};