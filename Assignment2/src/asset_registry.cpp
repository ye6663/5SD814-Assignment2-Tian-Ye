// asset_registry.cpp
#include "asset_registry.hpp"
#include "hash.hpp"
#include "filesystem.hpp"
#include <string_view>
#include <algorithm>

static std::string_view trim_left(std::string_view sv)
{
    const auto first = sv.find_first_not_of(" \t\n\r");
    if (first == std::string_view::npos) {
        return std::string_view{};
    }
    const auto last = sv.find_last_not_of(" \t\n\r");
    return sv.substr(first, (last - first + 1));
}

static void process_manifest_source(auto& table, const std::string_view source)
{
    size_t start_pos = 0;
    while (start_pos < source.length()) {
        size_t end_pos = source.find('\n', start_pos);
        if (end_pos == std::string_view::npos) {
            end_pos = source.length();
        }

        std::string_view line = source.substr(start_pos, end_pos - start_pos);
        line = trim_left(line);

        if (line.empty() || line[0] == '#') {
            start_pos = end_pos + 1;
            continue;
        }

        size_t separator_pos = line.find_first_of(" \t");
        if (separator_pos != std::string_view::npos) {
            std::string_view key = trim_left(line.substr(0, separator_pos));
            std::string_view value = trim_left(line.substr(separator_pos + 1));
            if (!key.empty()) {
                auto key_hash = fnv1a_64(key);
                table[key_hash] = value;
            }
        }

        start_pos = end_pos + 1;
    }
}

void AssetRegistry::clear()
{
    m_lookup.clear();
}

bool AssetRegistry::load_manifest(std::string_view path)
{
    std::string content;
    if (!load_file_content(path, content)) {
        return false;
    }

    process_manifest_source(m_lookup, content);
    return !m_lookup.empty();
}

bool AssetRegistry::contains(std::string_view name) const
{
    const uint64_t name_hash = fnv1a_64(name);
    return m_lookup.contains(name_hash);
}

void AssetRegistry::insert(std::string_view name, std::string_view path)
{
    const uint64_t name_hash = fnv1a_64(name);
    m_lookup.emplace(name_hash, path);
}

std::string_view AssetRegistry::get_asset_path(std::string_view name) const
{
    const uint64_t name_hash = fnv1a_64(name);
    if (m_lookup.contains(name_hash)) {
        const std::string& path = m_lookup.at(name_hash);
        return std::string_view{ path };
    }
    return std::string_view{};
}