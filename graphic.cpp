#include "graphic.h"

#include <cmath>
#include <stdexcept>
#define NOMINMAX
#define NODRAWTEXT
#include <windows.h>

constexpr BufferCell default_buffer_cell;
constexpr CanvasCell default_canvas_cell;

bool Graphic::IsInBounds(int x, int y) {
    return (
        0 <= x && x < static_cast<int>(width) &&
        0 <= y && y < static_cast<int>(height)
    );
}

Graphic::Graphic(Screen &screen) : _screen(screen), width(screen.width), height(screen.height << 1) {
    _buffer = new BufferCell[width * _screen.height];
    _canvas = new CanvasCell[width * height];
}

Graphic::~Graphic() {
    delete[] _canvas;
    delete[] _buffer;
}

void Graphic::Clear() {
    for (uint32_t i = 0; i < width * _screen.height; i++)
        _buffer[i] = default_buffer_cell;
    for (uint32_t i = 0; i < width * height; i++)
        _canvas[i] = default_canvas_cell;
}

void Graphic::SetCharacter(char character) { _context.cell.character = character; }
void Graphic::ResetCharacter() { _context.cell.character = default_buffer_cell.character; }

void Graphic::SetDimMode() { _context.cell.is_dim = true; }
void Graphic::ResetDimMode() { _context.cell.is_dim = false; }

void Graphic::SetTrueColor() { _context.cell.color_mode = EColorMode::kTrueColor; }
void Graphic::ResetTrueColor() { _context.cell.color_mode = EColorMode::kDefault; }

void Graphic::SetForegroundColor(EColor color) { ResetTrueColor(); _context.cell.foreground_color = color; }
void Graphic::ResetForegroundColor() { _context.cell.foreground_color = default_buffer_cell.foreground_color; }

void Graphic::SetBackgroundColor(EColor color) { ResetTrueColor(); _context.cell.background_color = color; }
void Graphic::ResetBackgroundColor() { _context.cell.background_color = default_buffer_cell.background_color; }

void Graphic::SetForegroundTrueColor(const Color& color) { SetTrueColor(); _context.cell.foreground_true_color = color; }
void Graphic::ResetForegroundTrueColor() { _context.cell.foreground_true_color = default_buffer_cell.foreground_true_color; }

void Graphic::SetBackgroundTrueColor(const Color& color) { SetTrueColor(); _context.cell.background_true_color = color; }
void Graphic::ResetBackgroundTrueColor() { _context.cell.background_true_color = default_buffer_cell.background_true_color; }

void Graphic::DrawText(int x, int y, const std::string &text) {
    x += _context.x;
    y += _context.y >> 1;
    if (!IsInBounds(x, y)) return;

    for (size_t i = 0, xi = x, yi = y; i < text.length(); i++) {
        _buffer[yi * width + xi] = _context.cell;
        _buffer[yi * width + xi].is_empty = false;
        _buffer[yi * width + xi].character = text[i];
        if (++xi >= width) {
            xi = 0;
            if (++yi >= height) break;
        }
    }
}

void Graphic::DrawTextWithFormat(int x, int y, const char* format, ...) {
    x += _context.x;
    y += _context.y >> 1;
    if (!IsInBounds(x, y)) return;

    va_list args;
    va_start(args, format);

    uint32_t len = vsnprintf(nullptr, 0, format, args);
    if (len <= 0) { throw std::runtime_error( "Error during formatting." ); }
    char* buf = new char[len + 1];
    vsnprintf(buf, len + 1, format, args);
    for (uint32_t i = 0, xi = x, yi = y; i < len; i++) {
        _buffer[yi * width + xi] = _context.cell;
        _buffer[yi * width + xi].is_empty = false;
        _buffer[yi * width + xi].character = buf[i];
        if (++xi >= width) {
            xi = 0;
            if (++yi >= height) break;
        }
    }
    
    va_end(args);
}

void Graphic::DrawPoint(int x, int y, const Color &color) {
    x += _context.x;
    y += _context.y;
    if (!IsInBounds(x, y)) return;
    CanvasCell* cell = _canvas + y * width + x;
    if (_context.z_index < cell->z_index) return;
    cell->z_index = _context.z_index;
    if (color.a == 255)
        _canvas[y * width + x] = { false, color, _context.z_index };
    else if (color.a == 0);
    else {
        double alpha = color.a / 255.0;
        cell->color.r = static_cast<int>(cell->color.r + (color.r - cell->color.r) * alpha);
        cell->color.g = static_cast<int>(cell->color.g + (color.g - cell->color.g) * alpha);
        cell->color.b = static_cast<int>(cell->color.b + (color.b - cell->color.b) * alpha);
        cell->color.a = 255;
        cell->z_index = _context.z_index;
    }
}

void Graphic::DrawLine(int x1, int y1, int x2, int y2, const Color &color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawPoint(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void Graphic::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color &color) {
    // 28.4 fixed-point coordinates
    const int Y1 = std::lround(16.0f * y1);
    const int Y2 = std::lround(16.0f * y2);
    const int Y3 = std::lround(16.0f * y3);

    const int X1 = std::lround(16.0f * x1);
    const int X2 = std::lround(16.0f * x2);
    const int X3 = std::lround(16.0f * x3);

    // Deltas
    const int DX12 = X1 - X2;
    const int DX23 = X2 - X3;
    const int DX31 = X3 - X1;

    const int DY12 = Y1 - Y2;
    const int DY23 = Y2 - Y3;
    const int DY31 = Y3 - Y1;

    // Fixed-point deltas
    const int FDX12 = DX12 << 4;
    const int FDX23 = DX23 << 4;
    const int FDX31 = DX31 << 4;

    const int FDY12 = DY12 << 4;
    const int FDY23 = DY23 << 4;
    const int FDY31 = DY31 << 4;

    // Bounding rectangle
    int minx = (std::min(X1, std::min(X2, X3)) + 0xF) >> 4;
    int maxx = (std::max(X1, std::max(X2, X3)) + 0xF) >> 4;
    int miny = (std::min(Y1, std::min(Y2, Y3)) + 0xF) >> 4;
    int maxy = (std::max(Y1, std::max(Y2, Y3)) + 0xF) >> 4;

    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    CanvasCell* colorBuffer = _canvas;
    colorBuffer += miny * width;

    // Half-edge constants
    int C1 = DY12 * X1 - DX12 * Y1;
    int C2 = DY23 * X2 - DX23 * Y2;
    int C3 = DY31 * X3 - DX31 * Y3;

    // Correct for fill convention
    if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
    if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
    if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;

    // Loop through blocks
    for(int y = miny; y < maxy; y += q)
    {
        for(int x = minx; x < maxx; x += q)
        {
            // Corners of block
            int x0 = x << 4;
            int x1 = (x + q - 1) << 4;
            int y0 = y << 4;
            int y1 = (y + q - 1) << 4;

            // Evaluate half-space functions
            bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
            bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
            bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
            bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
            int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);
    
            bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
            bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
            bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
            bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
            int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);
    
            bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
            bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
            bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
            bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
            int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

            // Skip block when outside an edge
            if(a == 0x0 || b == 0x0 || c == 0x0) continue;

            CanvasCell* buffer = colorBuffer;

            // Accept whole block when totally covered
            if(a == 0xF && b == 0xF && c == 0xF)
            {
                for(int iy = y; iy < y + q; iy++)
                {
                    for(int ix = x; ix < x + q; ix++)
                    {
                        DrawPoint(ix, iy, color);
                    }

                    buffer += width;
                }
            }
            else   // Partially covered block
            {
                int CY1 = C1 + DX12 * y0 - DY12 * x0;
                int CY2 = C2 + DX23 * y0 - DY23 * x0;
                int CY3 = C3 + DX31 * y0 - DY31 * x0;

                for(int iy = y; iy < y + q; iy++)
                {
                    int CX1 = CY1;
                    int CX2 = CY2;
                    int CX3 = CY3;
      
                    for(int ix = x; ix < x + q; ix++)
                    {
                        if(CX1 > 0 && CX2 > 0 && CX3 > 0)
                        {
                            DrawPoint(ix, iy, color);
                        }

                        CX1 -= FDY12;
                        CX2 -= FDY23;
                        CX3 -= FDY31;
                    }

                    CY1 += FDX12;
                    CY2 += FDX23;
                    CY3 += FDX31;

                    buffer += width;
                }
            }
        }

        colorBuffer += q * width;
    }
}

void Graphic::DrawRectangle(int x, int y, int width, int height, const Color &color) {
    DrawLine(x, y, x + width - 1, y, color);
    DrawLine(x, y, x, y + height - 1, color);
    DrawLine(x + width - 1, y, x + width - 1, y + height - 1, color);
    DrawLine(x, y + height - 1, x + width - 1, y + height - 1, color);
}

void Graphic::FillRectangle(int x, int y, int width, int height, const Color &color) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            DrawPoint(x + c, y + r, color);
        }
    }
}

void Graphic::DrawTexture(int x, int y, Texture &texture) {
    Color color;
    for (int r = 0; r < texture.height; r++) {
        for (int c = 0; c < texture.width; c++) {
            texture.GetColor(c, r, color);
            DrawPoint(x + c, y + r, color);
        }
    }
}

void Graphic::RenderCanvas() {
    for (uint32_t r = 0; r < _screen.height; r++) {
        for (uint32_t c = 0; c < width; c++) {
            CanvasCell* top = _canvas + (r << 1) * width + c;
            CanvasCell* bottom = top + width;
            if (top->is_empty && bottom->is_empty) continue;
            BufferCell* cell = _buffer + r * width + c;

            cell->is_empty = false;
            cell->color_mode = EColorMode::kTrueColor;
            if (top->is_empty) {
                cell->character = -2; // '?' U+2584
                cell->foreground_true_color = bottom->color;
                cell->background_true_color = { 0, 0, 0, 0 };
            }
            else if (bottom->is_empty) {
                cell->character = -1; // '?' U+2580
                cell->foreground_true_color = top->color;
                cell->background_true_color = { 0, 0, 0, 0 };
            }
            else {
                cell->character = -3; // �Ѵ�
                cell->foreground_true_color = top->color;
                cell->background_true_color = bottom->color;
            }
        }
    }
}

void Graphic::RenderBuffer() {
    std::string output;
    output.reserve(width * height * 30);

    bool is_empty_continued = false;
    BufferCell* prev_cell = nullptr;

    for (uint32_t r = 0; r < _screen.height; r++) {
        for (uint32_t c = 0; c < width; c++) {
            auto& cell = _buffer[r * width + c];
            if (cell.is_empty) {
                if (!is_empty_continued) {
                    output += "\x1b[39;49m";
                    is_empty_continued = true;
                }
                output += ' ';
                prev_cell = nullptr;
                continue;
            }

            is_empty_continued = false;

            bool is_color_changed = !prev_cell ||
                cell.color_mode != prev_cell->color_mode ||
                (
                    cell.color_mode == EColorMode::kTrueColor
                    ? (
                        cell.foreground_true_color.r != prev_cell->foreground_true_color.r ||
                        cell.foreground_true_color.g != prev_cell->foreground_true_color.g ||
                        cell.foreground_true_color.b != prev_cell->foreground_true_color.b ||
                        cell.background_true_color.r != prev_cell->background_true_color.r ||
                        cell.background_true_color.g != prev_cell->background_true_color.g ||
                        cell.background_true_color.b != prev_cell->background_true_color.b
                    )
                    : (
                        cell.is_dim != prev_cell->is_dim ||
                        cell.foreground_color != prev_cell->foreground_color ||
                        cell.background_color != prev_cell->background_color
                    )
                );

            if (is_color_changed) {
                char buf[64];
                if (cell.color_mode == EColorMode::kTrueColor) {
                    if (cell.character == -1) {
                        snprintf(buf, sizeof(buf), "\x1b[38;2;%d;%d;%dm\x1b[49m",
                            cell.foreground_true_color.r, cell.foreground_true_color.g, cell.foreground_true_color.b);
                    }
                    else if (cell.character == -2) {
                        snprintf(buf, sizeof(buf), "\x1b[38;2;%d;%d;%dm\x1b[49m",
                            cell.foreground_true_color.r, cell.foreground_true_color.g, cell.foreground_true_color.b);
                    }
                    else if (cell.character == -3) {
                        snprintf(buf, sizeof(buf), "\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm",
                            cell.foreground_true_color.r, cell.foreground_true_color.g, cell.foreground_true_color.b,
                            cell.background_true_color.r, cell.background_true_color.g, cell.background_true_color.b);
                    }
                }
                else
                    snprintf(buf, sizeof(buf), "\x1b[%d;%d;%dm",
                        (cell.is_dim ? 2 : 22), (int)cell.foreground_color, (int)cell.background_color);
                output += buf;
            }

            if (cell.character == -1)
                output += "\xE2\x96\x80";
            else if (cell.character == -2)
                output += "\xE2\x96\x84";
            else if (cell.character == -3)
                output += "\xE2\x96\x80";
            else
                output += cell.character;
            
            prev_cell = &cell;
        }

        output += '\n';
        prev_cell = nullptr;
    }

    fwrite(output.data(), 1, output.size(), stdout);
}

void Graphic::Render() {
    RenderCanvas();
    RenderBuffer();
}

void Graphic::Save() {
    _contextStack.push(_context);
}

void Graphic::Restore() {
    if (_contextStack.empty()) return;
    _context = _contextStack.top();
    _contextStack.pop();
}

void Graphic::Translate(int dx, int dy) {
    _context.x += dx;
    _context.y += dy;
}
