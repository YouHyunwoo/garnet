#pragma once

#include <string>
#include <cstdint>
#include "color.h"

class Texture {
public:
    uint8_t *data;
    int width, height, channels;
    Texture(uint8_t *data, int width, int height, int channels);
    ~Texture();
    uint8_t* GetRawColor(uint32_t x, uint32_t y);
    void GetColor(uint32_t x, uint32_t y, Color &color);
    uint8_t* GetRawColorByUV(double u, double v);
    void GetColorByUV(double u, double v, Color &color);
    static Texture* Load(std::string filename);
};
