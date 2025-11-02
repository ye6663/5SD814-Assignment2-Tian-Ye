// entity.cpp

#include "entity.hpp"

Entity::Entity() {
}

void Entity::set_size(Vector2 size) {
    m_transform.set_size(size);
}

void Entity::set_position(Vector2 position) {
    m_transform.set_position(position);
}

void Entity::set_rotation(float rotation) {
    m_transform.set_rotation(rotation);
}

void Entity::set_texture(TextureHandle texture) {
    m_sprite.set_texture(texture);
}

void Entity::set_tint(Color tint) {
    m_sprite.set_tint(tint);
}

void Entity::set_layer(int layer) {
    m_layer = layer;
}