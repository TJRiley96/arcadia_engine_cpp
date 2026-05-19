#pragma once

#include <string>


namespace Color {
    struct Color4 {
        float r;
        float g;
        float b;
        float a;

        Color4 (float red, float green, float blue, float alpha)
            : r(red), g(green), b(blue), a(alpha) {}

        Color4(const char hex[7]) {
            if (hex[0] == '#') {
                r = std::stoi(std::string(hex + 1, 2), nullptr, 16) / 255.0f;
                g = std::stoi(std::string(hex + 3, 2), nullptr, 16) / 255.0f;
                b = std::stoi(std::string(hex + 5, 2), nullptr, 16) / 255.0f;
                a = 1.0f; // Default alpha to fully opaque
            } else {
                r = g = b = a = 1.0f; // Default to white if format is incorrect
            }
        }
    };
}