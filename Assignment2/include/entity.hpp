// entity.hpp

#pragma once
#include "transform.hpp"
#include "sprite.hpp"

class Entity {
public:
    Entity();

    void set_size(Vector2 size);
    void set_position(Vector2 position);
    void set_rotation(float rotation);
    void set_texture(TextureHandle texture);
    void set_tint(Color tint);
    void set_layer(int layer);

    transform& get_transform() { return m_transform; }
    Sprite& get_sprite() { return m_sprite; }
    const transform& get_transform() const { return m_transform; }
    const Sprite& get_sprite() const { return m_sprite; }

    int get_layer() const { return m_layer; }

private:
    transform m_transform;
    Sprite m_sprite;
    int m_layer = 0;
};