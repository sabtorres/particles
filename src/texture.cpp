#include <texture.hpp>
#include <system_error>
#include <lodepng/lodepng.h>

Texture::Texture() {
    filename = "../resources/tex1.png";
    std::vector<unsigned char> png;

    unsigned error = lodepng::load_file(png, filename);
    if (error != 0) {
        throw std::runtime_error(lodepng_error_text(error));
    }

    error = lodepng::decode(data, width, height, png);
}

Texture::~Texture() {
    
}