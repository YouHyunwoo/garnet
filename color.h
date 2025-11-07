#pragma once

#include <cstdint>

enum class EColor {
    kBackground = 10,
    kBlack = 30,
    kRed = 31,
    kGreen = 32,
    kYellow = 33,
    kBlue = 34,
    kMagenta = 35,
    kCyan = 36,
    kWhite = 37,
    kDefault = 39,
    kBackgroundBlack = 40,
    kBackgroundRed = 41,
    kBackgroundGreen = 42,
    kBackgroundYellow = 43,
    kBackgroundBlue = 44,
    kBackgroundMagenta = 45,
    kBackgroundCyan = 46,
    kBackgroundWhite = 47,
    kBackgroundDefault = 49,
};

struct Color {
    uint8_t r, g, b, a = 255;
};