#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "Color.h"
#include "Types.h"

using Types::Vertex3;
using Types::Vertex5;
using Types::Vertex8;
using Types::Index;

namespace Config {
    const int width = 800;
    const int height = 800;


    constexpr const char* windowTitle = "Arcadia Engine";
    constexpr const char* logDirectory = "logs";

    constexpr const char* vertShaderPath = "assets/shaders/SpriteVert.glsl";
    constexpr const char* fragShaderPath = "assets/shaders/SpriteFrag.glsl";

    constexpr const char* meshVertShaderPath = "assets/shaders/BasicMeshVert.glsl";
    constexpr const char* meshFragShaderPath = "assets/shaders/BasicMeshFrag.glsl";

    constexpr const char* DEFAULT_TEXTURE = "assets/textures/default.png";
    constexpr const char* COLOR_GRID_1024 = "assets/textures/Color_Grid_1024.png";
    constexpr const char* TEST_CUBE_PNG = "assets/textures/cube_test.png";

    const unsigned int texture_grid_size = 4;

    const unsigned int chunk_size = 16;
    const unsigned int chunk_height = 16;

    const float TRI_VERTS[9] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    const Types::Vertex3 TRI_VERTS_V[3] = {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f }
    };

    const Vertex3 QUAD_V[4] = {
        { -0.5f, 0.5f, 0.0f },
        {  0.5f, 0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };

    const Vertex5 QUAD_TEX[4] = {
        { -0.5f, 0.5f, 0.0f, 0.0f, 1.0f },
        {  0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f }
    };

    const Vertex8 QUAD_8[4] = {
        { -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
        {  0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
        {  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
    };

    const unsigned int spriteIndices[3] = { 0, 1, 2 };

    const Types::Index spriteIndicesV[1] = {{ 0, 1, 2 }};

    const Index QUAD_I[2] = {
        { 0, 1, 2 },
        { 2, 3, 0 }
    };
}