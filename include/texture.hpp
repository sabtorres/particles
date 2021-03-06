#pragma once

#include <string>

struct Texture {
    std::string filename;
    unsigned char* data;
    int width;
    int height;
    int number_of_components;

    Texture();
    ~Texture();
};