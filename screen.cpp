#include "screen.h"

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#define NODRAWTEXT
#include <windows.h>

Screen::Screen(uint32_t width, uint32_t height) :
    width(width), height(height),
    half_width(width >> 1), half_height(height >> 1),
    area(width * height),
    aspect_ratio((double)height / width) {
    SetConsoleOutputCP(CP_UTF8);
}

void Screen::Clear() { printf("\x1b[2J"); }

void Screen::ReturnCursor() { printf("\x1b[H"); }
void Screen::MoveCursor(int x, int y) { printf("\x1b[%d;%dH", y, x); }
