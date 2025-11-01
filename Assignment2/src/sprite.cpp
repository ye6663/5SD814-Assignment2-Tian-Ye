// sprite.cpp
#include "sprite.hpp"
#include "texture_manager.hpp"

Sprite::Sprite()
    : m_tint(WHITE)
{
}

Sprite::Sprite(TextureHandle texture)
    : m_texture(texture)
    , m_tint(WHITE)
{
}

void Sprite::set_texture(TextureHandle texture)
{
    m_texture = texture;
}

void Sprite::set_tint(Color tint)
{
    m_tint = tint;
}