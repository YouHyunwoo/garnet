#pragma once

#include <stack>
#include <string>
#include "screen.h"
#include "color.h"
#include "texture.h"

enum class EColorMode {
    kDefault,
    kTrueColor,
};

struct BufferCell {
    bool is_empty = true;
    char character = ' ';
    EColorMode color_mode = EColorMode::kDefault;
    bool is_dim = false;
    EColor foreground_color = EColor::kDefault;
    EColor background_color = EColor::kBackgroundDefault;
    Color foreground_true_color = { 255, 255, 255, 255 };
    Color background_true_color = { 0, 0, 0, 255 };
};

struct CanvasCell {
    bool is_empty = true;
    Color color = { 50, 50, 50, 255 };
    double z_index = 0.0;
};

struct GraphicContext {
    BufferCell cell;
    int32_t x = 0, y = 0;
    Color color = { 255, 255, 255, 255 };
    double z_index = 0.0;
};

class Graphic {
private:
    Screen &_screen;
    BufferCell *_buffer;
    CanvasCell *_canvas;
    GraphicContext _context;
    std::stack<GraphicContext> _contextStack;
    bool IsInBounds(int x, int y);
    void RenderCanvas();
    void RenderBuffer();
public:
    uint32_t width, height;
    Graphic(Screen &screen);
    ~Graphic();
    void Clear();
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
    void SetZIndex(double z_index);
    void DrawText(int x, int y, const std::string &text);
    void DrawTextWithFormat(int x, int y, const char* format, ...);
    void DrawPoint(int x, int y, const Color &color);
    void DrawLine(int x1, int y1, int x2, int y2, const Color &color);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color &color);
    void DrawRectangle(int x, int y, int width, int height, const Color &color);
    void FillRectangle(int x, int y, int width, int height, const Color &color);
    void DrawTexture(int x, int y, Texture &texture);
    void Render();
    void Save();
    void Restore();
    void Translate(int dx, int dy);
};
