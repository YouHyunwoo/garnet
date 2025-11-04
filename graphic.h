#ifndef __GRAPHIC_H_
#define __GRAPHIC_H_

#include "screen.h"

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
    void Flush();
    void DrawPoint(int x, int y, const Color& color);
};

#endif