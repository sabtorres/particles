#pragma once

#include <string>
#include <vector>

struct Texture {
    std::string filename;
    std::vector<unsigned char> data;
    uint width;
    uint height;

    Texture();
    Texture(const std::string& filename);
    ~Texture();
    unsigned char random_uniform();
};