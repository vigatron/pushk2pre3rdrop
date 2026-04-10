#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

bool fileExists(const std::string &path);

std::vector<uint8_t> load_binary_file(const std::string &filename);
