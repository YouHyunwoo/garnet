#include "log.h"

bool Log::is_visible = true;
std::string Log::buffer = "";

void Log::Render(Graphic& graphic) {
    if (!is_visible) { return; }

    graphic.Save();
    graphic.FillRectangle(0, graphic.height - 20, graphic.width, 20, {0, 0, 0, 255});
    graphic.DrawLine(0, graphic.height - 20, graphic.width, graphic.height - 20, {255, 255, 255, 255});

    graphic.ResetTrueColor();
    graphic.ResetForegroundColor();
    graphic.ResetBackgroundColor();
    int y_offset = graphic.height / 2 - 9;
    int y = 10;
    size_t start = buffer.rfind('\n');
    size_t end = buffer.length();
    while (start != std::string::npos) {
        std::string line = buffer.substr(start + 1, end - start - 1);
        int lines = static_cast<int>(line.length() / graphic.width + 1);
        y -= lines;
        for (int i = 0; i < lines; i++) {
            std::string subline = line.substr(i * graphic.width, graphic.width);
            if (y + i >= 0)
            graphic.DrawText(0, y + y_offset + i, subline);
        }
        end = start;
        start = buffer.rfind('\n', start - 1);
    }
    std::string line = buffer.substr(start + 1, end - start - 1);
    int lines = static_cast<int>(line.length() / graphic.width + 1);
    y -= lines;
    for (int i = 0; i < lines; i++) {
        std::string subline = line.substr(i * graphic.width, graphic.width);
        if (y + i >= 0)
            graphic.DrawText(0, y + y_offset + i, subline);
    }

    graphic.Restore();
}