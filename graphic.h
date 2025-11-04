#ifndef __GRAPHIC_H_
#define __GRAPHIC_H_

#include "screen.h"
#include <string>

class Graphic {
private:
    Color* _canvas;
    Screen& _screen;
    bool IsInBounds(int x, int y);
public:
    int width, height;
    Graphic(Screen& screen);
    ~Graphic();
    void Clear();
    void DrawPoint(int x, int y, const Color& color);
    void Flush();
};

class Texture {
private:
    uint8_t* _data;
public:
    int width, height, channels;
    Texture(uint8_t* data, int width, int height, int channels);
    ~Texture();
    uint8_t* GetRawColor(double u, double v);
    void GetColor(double u, double v, Color& color);
    static Texture* Load(std::string filename);
};

#endif