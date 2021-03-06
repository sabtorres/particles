#include <texture.hpp>
#include <system_error>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
    filename = "../resources/tex1.png";
    data = stbi_load(filename.c_str(), &width, &height,
        &number_of_components, 0);

	if(stbi_failure_reason()) {
		throw std::runtime_error(stbi_failure_reason());
	}
}

Texture::~Texture() {
    stbi_image_free(data);
}