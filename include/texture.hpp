#pragma once

#include <string>
#include <vector>

struct Texture {
    const char* filename;
    std::vector<unsigned char> data;
    uint width;
    uint height;

    Texture();
    ~Texture();
};