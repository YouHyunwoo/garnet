#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stack>

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
    uint8_t r, g, b;
};

struct BufferCell {
    bool is_empty = true;
    char character = ' ';
    bool is_dim = false;
    bool is_true_color = false;
    EColor foreground_color = EColor::kDefault;
    EColor background_color = EColor::kBackgroundDefault;
    Color foreground_tcolor = { 200, 200, 200 };
    Color background_tcolor = { 0, 0, 0 };
    float z_index = 0;
};

typedef BufferCell GraphicContext;

class Screen {
private:
    BufferCell** _buffer;
    GraphicContext _context;
    std::stack<GraphicContext> _context_stack;

    bool IsInBounds(uint32_t x, uint32_t y);
public:
    const uint32_t width, height;
    const uint32_t half_width, half_height;
    const uint32_t area;
    const double aspect_ratio;

    Screen(uint32_t width, uint32_t height);
    ~Screen();
    void ClearScreen();
    void ClearBuffer();
    void RenderBuffer();
    bool IsBufferEmpty(int x, int y);
    void DrawPoint(int x, int y);
    void DrawPointWithZIndex(int x, int y, float z_index);
    void DrawTextWithFormat(int x, int y, const char* format, ...);
    void ReturnCursor();
    void MoveCursor(int x, int y);
    void SetCharacter(char character);
    void ResetCharacter();
    void SetDimMode();
    void ResetDimMode();
    void SetTrueColor();
    void ResetTrueColor();
    void SetForegroundColor(EColor color);
    void ResetForegroundColor();
    void SetBackgroundColor(EColor color);
    void ResetBackgroundColor();
    void SetForegroundTrueColor(const Color& color);
    void ResetForegroundTrueColor();
    void SetBackgroundTrueColor(const Color& color);
    void ResetBackgroundTrueColor();
    void SetZIndex(float z_index);
    void ResetZIndex();
    void SaveContext();
    void RestoreContext();
};

#endif