#pragma once



// Built-in includes

// Third-party includes
#include <glad/glad.h>

// Local includes

enum FilterMode {
    NEAREST= GL_NEAREST,
    LINEAR= GL_LINEAR,
    NEAREST_MIPMAP_NEAREST= GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST= GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR= GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR= GL_LINEAR_MIPMAP_LINEAR
};


class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const char* filePath, FilterMode filterMode);
    void Unload();

    void Bind();
    void Unbind() const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    unsigned int textureID;
    int width;
    int height;
    int channels;
};