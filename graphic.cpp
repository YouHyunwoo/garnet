#include "graphic.h"

#include <cstring>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Graphic::IsInBounds(int x, int y) {
    return (
        0 <= x && x < width &&
        0 <= y && y < height
    );
}

Graphic::Graphic(Screen& screen) : _screen(screen), width(screen.width), height(screen.height * 2) {
    _canvas = new Color[screen.width * screen.height * 2];
    memset(_canvas, 0, sizeof(Color) * width * height);
}

Graphic::~Graphic() {
    delete _canvas;
}

void Graphic::Clear() {
    memset(_canvas, 0, sizeof(Color) * width * height);
}

void Graphic::DrawPoint(int x, int y, const Color& color) {
    if (!IsInBounds(x, y)) return;
    _canvas[y * width + x] = color;
}

void Graphic::Flush() {
    _screen.SaveContext();
    _screen.SetTrueColor();
    _screen.SetCharacter(-1);
    int index = 0;
    for (int r = 0; r < height; r += 2) {
        for (int c = 0; c < width; c++) {
            _screen.SetForegroundTrueColor(_canvas[index]);
            _screen.SetBackgroundTrueColor(_canvas[index + width]);
            _screen.DrawPoint(c, r / 2);
            index++;
        }
        index += width;
    }
    _screen.RestoreContext();
}

Texture::Texture(uint8_t* data, int width, int height, int channels)
    : _data(data), width(width), height(height), channels(channels) {
}

Texture::~Texture()
{
    free(_data);
}

uint8_t* Texture::GetRawColor(double u, double v) {
    return _data + ((int)floor(v * height) * width + (int)floor(u * width)) * channels;
}

void Texture::GetColor(double u, double v, Color& out) {
    memcpy(&out, GetRawColor(u, v), 3);
}

Texture* Texture::Load(std::string filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) return nullptr;
    Texture* result = new Texture(data, width, height, channels);
    return result;
}