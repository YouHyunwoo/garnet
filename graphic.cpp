#include "graphic.h"

#include <cmath>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Graphic::IsInBounds(int x, int y) {
    return (
        0 <= x && x < width &&
        0 <= y && y < height
    );
}

Graphic::Graphic(Screen &screen) : _screen(screen), width(screen.width), height(screen.height * 2) {
    _canvas = new Color[screen.width * screen.height * 2];
    memset(_canvas, 0, sizeof(Color) * width * height);
}

Graphic::~Graphic() {
    delete _canvas;
}

void Graphic::Clear() {
    memset(_canvas, 0, sizeof(Color) * width * height);
}

void Graphic::DrawPoint(int x, int y, const Color &color) {
    if (!IsInBounds(x, y)) return;
    _canvas[y * width + x] = color;
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

    Color* colorBuffer = _canvas;
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

            Color* buffer = colorBuffer;

            // Accept whole block when totally covered
            if(a == 0xF && b == 0xF && c == 0xF)
            {
                for(int iy = y; iy < y + q; iy++)
                {
                    for(int ix = x; ix < x + q; ix++)
                    {
                        _canvas[ix] = color;
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
                            buffer[ix] = color;
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

void Graphic::DrawTexture(int x, int y, Texture &texture) {
    Color* buffer = _canvas + x + y * width;
    uint8_t* raw_color = texture.GetRawColor(0, 0);
    for (int r = 0; r < texture.height; r++) {
        for (int c = 0; c < texture.width; c++) {
            buffer->r = *(raw_color++);
            buffer->g = *(raw_color++);
            buffer->b = *(raw_color++);
            buffer++;
        }
        buffer += width - texture.width;
    }
}

void Graphic::Flush() {
    _screen.SaveContext();
    _screen.SetTrueColor();
    _screen.SetCharacter(-1);
    Color* buffer = _canvas;
    int half_height = height >> 1;
    for (int r = 0; r < half_height; r++) {
        for (int c = 0; c < width; c++) {
            _screen.SetForegroundTrueColor(*buffer);
            _screen.SetBackgroundTrueColor(buffer[width]);
            _screen.DrawPoint(c, r);
            buffer++;
        }
        buffer += width;
    }
    _screen.RestoreContext();
}

Texture::Texture(uint8_t *data, int width, int height, int channels)
    : _data(data), width(width), height(height), channels(channels) {
}

Texture::~Texture()
{
    free(_data);
}

uint8_t* Texture::GetRawColor(double u, double v) {
    return _data + ((int)floor(v * height) * width + (int)floor(u * width)) * channels;
}

void Texture::GetColor(double u, double v, Color &out) {
    memcpy(&out, GetRawColor(u, v), 3);
}

Texture* Texture::Load(std::string filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) return nullptr;
    Texture* result = new Texture(data, width, height, channels);
    return result;
}