// texture_manager.cpp

#include "texture_manager.hpp"
#include "asset_registry.hpp"
#include "hash.hpp"
#include <raylib.h>
#include <cassert>
#include <iostream>

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    release_textures();
}

bool TextureManager::load_manifest(std::string_view path)
{
    return m_registry.load_manifest(path);
}

void TextureManager::release_textures()
{
    for (size_t index = 0; index < m_metadata.size(); index++) {
        const MetaData& meta = m_metadata[index];
        unload(TextureHandle{ (uint16_t)meta.index, meta.version });
    }

    m_metadata.clear();
    m_textures.clear();
}

bool TextureManager::contains(std::string_view name) const
{
    if (!m_registry.contains(name)) {
        return false;
    }

    std::string_view path = m_registry.get_asset_path(name);
    const uint64_t path_hash = fnv1a_64(path);
    for (const auto& meta : m_metadata) {
        if (meta.hash == path_hash) {
            return true;
        }
    }
    return false;
}

TextureHandle TextureManager::load(std::string_view name)
{
    if (!m_registry.contains(name)) {
        return INVALID_TEXTURE_HANDLE;
    }

    std::string_view path = m_registry.get_asset_path(name);

    // note: check if we've already loaded the texture
    const uint64_t path_hash = fnv1a_64(path);
    for (const auto& meta : m_metadata) {
        if (meta.hash == path_hash) {
            return TextureHandle{ meta.index, meta.version };
        }
    }

    // note: load the texture and add it
    Texture2D texture = LoadTexture(path.data());
    if (texture.id == 0) {
        std::cout << "Could not load texture '" << path << "'!" << std::endl;
        return INVALID_TEXTURE_HANDLE;
    }

    std::cout << "'" << path << "' loaded." << std::endl;

    constexpr uint16_t VERSION_START = 1;
    const uint16_t data_index = (uint16_t)m_metadata.size();
    m_metadata.emplace_back(MetaData{ std::string(path), path_hash, data_index, VERSION_START });
    m_textures.emplace_back(std::move(texture));

    const MetaData& meta = m_metadata.back();
    return TextureHandle{ meta.index, meta.version };
}

void TextureManager::unload(TextureHandle handle)
{
    // note: validating the handle through some basic checks
    const uint16_t last_valid_index = (uint16_t)m_metadata.size();
    if (handle.index >= last_valid_index) {
        std::cout << "Trying to unload a texture using an invalid handle - index (" << handle.index << ", " << handle.version << ")." << std::endl;
        return;
    }

    MetaData& meta = m_metadata[handle.index];
    if (handle.version != meta.version) {
        std::cout << "Trying to unload a texture using an invalid handle - version (" << handle.index << ", " << handle.version << ")." << std::endl;
        return;
    }

    std::cout << "'" << meta.path << "' unloaded." << std::endl;

    Texture2D& texture = m_textures[handle.index];
    UnloadTexture(texture);
    texture = Texture2D{};
}

Texture2D* TextureManager::get_texture_from_handle(TextureHandle handle)
{
    // note: validating the handle through some basic checks
    const uint16_t last_valid_index = (uint16_t)m_metadata.size();
    if (handle.index >= last_valid_index) {
        std::cout << "Trying to access a texture using an invalid handle - index (" << handle.index << ", " << handle.version << ")." << std::endl;
        return nullptr;
    }

    const MetaData& meta = m_metadata[handle.index];
    if (handle.version != meta.version) {
        std::cout << "Trying to access a texture using an invalid handle - version (" << handle.index << ", " << handle.version << ")." << std::endl;
        return nullptr;
    }

    return &m_textures[handle.index];
}