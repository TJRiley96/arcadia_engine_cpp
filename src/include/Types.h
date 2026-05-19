#pragma once

// Built-in includes
#include <stdint.h>
#include <array>

// Third-party includes

// Local includes
#include "Vector.h"

using Vector::Vector3;
using Vector::Vector2;

namespace Types {

    // Vertex definitions
    // 3D vertex with position only
    typedef std::array<float, 3> Vertex3;
    // 5D vertex with position and texture coordinates
    typedef std::array<float, 5> Vertex5;
    // 8D vertex with position, texture coordinates, and color
    typedef std::array<float, 8> Vertex8;

    typedef std::array<unsigned int, 3> Index;

    typedef std::array<float, 3> Normal;

    typedef std::array<float, 2> UV;

    typedef uint8_t KeyCode;

    inline void CreateVertex8(const Vector3& position, const Vector3& normal, const Vector2& texCoords, Vertex8& vertex) {
        vertex[0] = position.x;
        vertex[1] = position.y;
        vertex[2] = position.z;
        vertex[3] = normal.x;
        vertex[4] = normal.y;
        vertex[5] = normal.z;
        vertex[6] = texCoords.x;
        vertex[7] = texCoords.y;
    }

    inline void CreateVertex8(const Vertex3& position, const Normal& normal, const UV& texCoords, Vertex8& vertex) {
        vertex[0] = position[0];
        vertex[1] = position[1];
        vertex[2] = position[2];
        vertex[3] = normal[0];
        vertex[4] = normal[1];
        vertex[5] = normal[2];
        vertex[6] = texCoords[0];
        vertex[7] = texCoords[1];
    }
}