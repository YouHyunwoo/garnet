#include "texture.h"

#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(uint8_t *data, int width, int height, int channels)
    : data(data), width(width), height(height), channels(channels) {
}

Texture::~Texture()
{
    free(data);
}

uint8_t* Texture::GetRawColor(uint32_t x, uint32_t y) {
    return data + (y * width + x) * channels;
}

void Texture::GetColor(uint32_t x, uint32_t y, Color &out) {
    memcpy(&out, GetRawColor(x, y), channels);
}

uint8_t* Texture::GetRawColorByUV(double u, double v) {
    return data + ((int)floor(v * height) * width + (int)floor(u * width)) * channels;
}

void Texture::GetColorByUV(double u, double v, Color &out) {
    memcpy(&out, GetRawColorByUV(u, v), channels);
}

Texture* Texture::Load(std::string filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) return nullptr;
    Texture* result = new Texture(data, width, height, channels);
    return result;
}
