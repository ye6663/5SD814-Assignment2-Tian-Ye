// asset_registry.hpp

#pragma once
#include <string_view>
#include <string>
#include <unordered_map>
#include <cstdint>

class AssetRegistry {
public:
    AssetRegistry() = default;

    void clear();
    bool load_manifest(std::string_view path);
    bool contains(std::string_view name) const;
    void insert(std::string_view name, std::string_view path);
    std::string_view get_asset_path(std::string_view name) const;

private:
    std::unordered_map<uint64_t, std::string> m_lookup;
};