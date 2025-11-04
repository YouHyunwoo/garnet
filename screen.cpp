#include "screen.h"

#include <memory>
#include <string>
#include <stdexcept>
#include <windows.h>

BufferCell default_cell;

bool Screen::IsInBounds(uint32_t x, uint32_t y) {
    return (
        0 <= x && x < width &&
        0 <= y && y < height
    );
}

Screen::Screen(uint32_t width, uint32_t height) :
    width(width), height(height),
    half_width(width >> 1), half_height(height >> 1),
    area(width * height),
    aspect_ratio((double)height / width) {
    _buffer = new BufferCell*[height];

    for (uint32_t r = 0; r < height; r++)
        _buffer[r] = new BufferCell[width]; // TODO: default value check

    SetConsoleOutputCP(CP_UTF8);
}

Screen::~Screen() {
    for (uint32_t r = 0; r < height; r++)
        delete[] _buffer[r];
    delete[] _buffer;
}

void Screen::ClearScreen() { printf("\x1b[2J"); }

void Screen::ClearBuffer() {
    for (uint32_t r = 0; r < height; r++)
        for (uint32_t c = 0; c < width; c++)
            _buffer[r][c] = default_cell;
}

void Screen::RenderBuffer() {
    bool is_empty_continued = false;

    for (uint32_t r = 0; r < height; r++) {
        for (uint32_t c = 0; c < width; c++) {
            auto& cell = _buffer[r][c];
            if (cell.is_empty) {
                if (!is_empty_continued) {
                    printf("\x1b[39;49m");
                    is_empty_continued = true;
                }
                putchar(' ');
                continue;
            }

            is_empty_continued = false;

            if (cell.is_true_color)
                printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm",
                    cell.foreground_tcolor.r, cell.foreground_tcolor.g, cell.foreground_tcolor.b,
                    cell.background_tcolor.r, cell.background_tcolor.g, cell.background_tcolor.b);
            else
                printf("\x1b[%d;%d;%dm", (cell.is_dim ? 2 : 22), (int)cell.foreground_color, (int)cell.background_color);

            if (cell.character == -1)
                printf("\xE2\x96\x80");
            else
                putchar(cell.character);
        }
        putchar('\n');
    }
}

bool Screen::IsBufferEmpty(int x, int y) {
    return IsInBounds(x, y) && _buffer[y][x].is_empty;
}

void Screen::DrawPoint(int x, int y) {
    if (!IsInBounds(x, y)) return;

    _buffer[y][x] = _context;
    _buffer[y][x].is_empty = false;
}

void Screen::DrawPointWithZIndex(int x, int y, float z_index) {
    if (!IsInBounds(x, y)) return;

    if (_buffer[y][x].is_empty || _buffer[y][x].z_index < z_index) {
        _buffer[y][x] = _context;
        _buffer[y][x].is_empty = false;
        _buffer[y][x].z_index = z_index;
    }
}

void Screen::DrawTextWithFormat(int x, int y, const char* format, ...) {
    if (!IsInBounds(x, y)) return;

    va_list args;
    va_start(args, format);

    uint32_t len = vsnprintf(nullptr, 0, format, args);
    if (len <= 0) { throw std::runtime_error( "Error during formatting." ); }
    char* buf = new char[len + 1];
    vsnprintf(buf, len + 1, format, args);
    for (uint32_t i = 0, xi = x, yi = y; i < len; i++) {
        _buffer[yi][xi] = _context;
        _buffer[yi][xi].is_empty = false;
        _buffer[yi][xi].character = buf[i];
        if (++xi >= width) {
            xi = 0;
            if (++yi >= height) break;
        }
    }
    
    va_end(args);
}

void Screen::ReturnCursor() { printf("\x1b[H"); }
void Screen::MoveCursor(int x, int y) { printf("\x1b[%d;%dH", y, x); }

void Screen::SetCharacter(char character) { _context.character = character; }
void Screen::ResetCharacter() { _context.character = default_cell.character; }

void Screen::SetDimMode() { _context.is_dim = true; }
void Screen::ResetDimMode() { _context.is_dim = false; }

void Screen::SetTrueColor() { _context.is_true_color = true; }
void Screen::ResetTrueColor() { _context.is_true_color = false; }

void Screen::SetForegroundColor(EColor color) { ResetTrueColor(); _context.foreground_color = color; }
void Screen::ResetForegroundColor() { _context.foreground_color = default_cell.foreground_color; }

void Screen::SetBackgroundColor(EColor color) { ResetTrueColor(); _context.background_color = color; }
void Screen::ResetBackgroundColor() { _context.background_color = default_cell.background_color; }

void Screen::SetForegroundTrueColor(const Color& color) { SetTrueColor(); _context.foreground_tcolor = color; }
void Screen::ResetForegroundTrueColor() { _context.foreground_tcolor = default_cell.foreground_tcolor; }

void Screen::SetBackgroundTrueColor(const Color& color) { SetTrueColor(); _context.background_tcolor = color; }
void Screen::ResetBackgroundTrueColor() { _context.background_tcolor = default_cell.background_tcolor; }

void Screen::SetZIndex(float z_index) { _context.z_index = z_index; }
void Screen::ResetZIndex() { _context.z_index = default_cell.z_index; }

void Screen::SaveContext() {
    _context_stack.push(_context);
}
void Screen::RestoreContext() {
    _context = _context_stack.top();
    _context_stack.pop();
}