#pragma once

#include <string>
#include <iostream>
#include "graphic.h"

class Log {
public:
    static bool is_visible;
    static std::string buffer;
    static void Render(Graphic& graphic);
    static void Info(const std::string& message) {
        buffer += "[INFO] " + message + "\n";
    }

    static void Warning(const std::string& message) {
        buffer += "[WARNING] " + message + "\n";
    }

    static void Error(const std::string& message) {
        buffer += "[ERROR] " + message + "\n";
    }
};