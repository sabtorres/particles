#include <texture.hpp>
#include <system_error>
#include <random>
#include <lodepng/lodepng.h>

const int RANDOM_NOISE_DIMS = 256;

Texture::Texture() {
    for (int i = 0; i < RANDOM_NOISE_DIMS * RANDOM_NOISE_DIMS ; i++) {
        data.push_back(random_uniform());
    }
    width = RANDOM_NOISE_DIMS;
    height = RANDOM_NOISE_DIMS;
    filename = "";
}

Texture::Texture(const std::string& filename) {
    std::vector<unsigned char> png;
    this->filename = filename;

    unsigned error = lodepng::load_file(png, filename);
    if (error != 0) {
        throw std::runtime_error(lodepng_error_text(error));
    }

    error = lodepng::decode(data, width, height, png);
}

Texture::~Texture() {
    
}

unsigned char Texture::random_uniform() {
    return static_cast<unsigned char>((std::rand() % 256));
}