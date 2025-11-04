#include "graphic.h"

#include <cstring>

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

void Graphic::Flush() {
    _screen.SaveContext();
    _screen.SetTrueColor();
    _screen.SetCharacter(-1);
    int index = 0;
    for (int r = 0; r < height; r += 2) {
        for (int c = 0; c < width; c++) {
            _screen.SetForegroundTrueColor(_canvas[index]);
            _screen.SetBackgroundTrueColor(_canvas[index + width]);
            _screen.DrawPoint(c, r >> 1);
            index++;
        }
        index += width;
    }
    _screen.RestoreContext();
}

void Graphic::DrawPoint(int x, int y, const Color& color) {
    if (!IsInBounds(x, y)) return;
    _canvas[y * width + x] = color;
}