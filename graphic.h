#ifndef __GRAPHIC_H_
#define __GRAPHIC_H_

#include "screen.h"
#include <string>

class Texture;

class Graphic {
private:
    Color *_canvas;
    Screen &_screen;
    bool IsInBounds(int x, int y);
public:
    int width, height;
    Graphic(Screen &screen);
    ~Graphic();
    void Clear();
    void DrawPoint(int x, int y, const Color &color);
    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color &color);
    void DrawTexture(int x, int y, Texture &texture);
    void Flush();
};

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

#endif