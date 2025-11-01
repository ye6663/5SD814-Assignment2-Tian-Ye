// texture_manager.hpp

#pragma once
#include "asset_registry.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <cstdint>

struct TextureHandle {
    uint16_t index = uint16_t(-1);
    uint16_t version = 0;
};

constexpr TextureHandle INVALID_TEXTURE_HANDLE;

class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    bool load_manifest(std::string_view path);
    void release_textures();
    bool contains(std::string_view name) const;
    TextureHandle load(std::string_view name);
    void unload(TextureHandle handle);
    Texture2D* get_texture_from_handle(TextureHandle handle);

private:
    struct MetaData {
        std::string path;
        uint64_t hash = 0;
        uint16_t index = 0;
        uint16_t version = 0;
    };
    std::vector<MetaData> m_metadata;
    std::vector<Texture2D> m_textures;

    AssetRegistry m_registry;
};