#pragma once

#include <cstdint>

class Screen {
public:
    const uint32_t width, height;
    const uint32_t half_width, half_height;
    const uint32_t area;
    const double aspect_ratio;

    Screen(uint32_t width, uint32_t height);
    void Clear();
    void ReturnCursor();
    void MoveCursor(int x, int y);
};
