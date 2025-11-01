// filesystem.hpp
#pragma once
#include <string_view>
#include <string>
#include <functional>
#include <cstdint>

bool has_extension(std::string_view path, std::string_view ending);
void enumerate_folder_recursive(std::string_view path, std::function<void(std::string_view, uint64_t)> callback);
bool load_file_content(std::string_view path, std::string& content);
uint64_t get_last_write_timestamp(std::string_view path);