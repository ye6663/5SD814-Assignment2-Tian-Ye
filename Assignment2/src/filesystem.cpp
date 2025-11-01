// filesystem.cpp
#include "filesystem.hpp"
#include <filesystem>
#include <cstdio>

bool has_extension(std::string_view path, std::string_view ending)
{
    std::string_view extension = path.substr(path.find_last_of('.'));
    return extension.compare(ending.data()) == 0;
}

void enumerate_folder_recursive(std::string_view path, std::function<void(std::string_view, uint64_t)> callback)
{
    for (auto& it : std::filesystem::recursive_directory_iterator(path)) {
        if (!it.is_regular_file()) {
            continue;
        }

        const std::string& file_path = it.path().string();
        const uint64_t file_size = it.file_size();
        callback(file_path, file_size);
    }
}

bool load_file_content(std::string_view path, std::string& content)
{
    FILE* file = nullptr;
#ifdef _WIN32
    if (fopen_s(&file, path.data(), "rb") != 0) {
#else
    file = fopen(path.data(), "rb");
    if (!file) {
#endif
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    content.resize(size);
    size_t bytes = fread(content.data(), 1, content.size(), file);
    fclose(file);

    return bytes == size;
    }

uint64_t get_last_write_timestamp(std::string_view path)
{
    std::error_code error{};
    auto last_write = std::filesystem::last_write_time(path, error);
    if (error) {
        return 0;
    }
    auto epoch_time = last_write.time_since_epoch();
    auto epoch_ticks = epoch_time.count();
    return epoch_ticks;
}