#include "Texture.h"

// Built-in includes
#include <iostream>
#include <string>

// Third-party includes
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Local includes
#include "Logging.h"

Texture::Texture()
: textureID(0), width(0), height(0), channels(0) {}

Texture::~Texture() {
    Unload();
}

bool Texture::Load(const char* filePath, FilterMode filterMode) {
    // Load texture from file and create OpenGL texture object
    // Return true if successful, false otherwise

    // Texture loading code here (e.g., using stb_image or similar library)
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    // load image, create texture and generate mipmaps
    int loadedWidth, loadedHeight, loadedChannels;

    // stbi_set_flip_vertically_on_load(true); // Flip the image vertically on load to match OpenGL's coordinate system
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(filePath, &loadedWidth, &loadedHeight, &loadedChannels, 0);
    if (data)
    {
        width = loadedWidth;
        height = loadedHeight;
        channels = loadedChannels;

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        stbi_image_free(data);
        Logging::LogError("Failed to load texture data: %s", filePath);
        return false;
    }
    stbi_image_free(data);

    Logging::LogInfo("Texture loaded successfully");

    return true;
}


void Texture::Unload() {
    // Clean up texture resources
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}


void Texture::Bind() {
    // Bind the texture to a specific texture unit
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

