
#include "Renderer.h"

Renderer::Renderer(Game* game)
:game(game),
window(nullptr),
glContext(nullptr)
{
}

Renderer::~Renderer() {
    // Destructor code here
}

bool Renderer::Initialize(float width, float height) {
    windowWidth = width;
    windowHeight = height;

    window = SDL_CreateWindow(
        "Arcadia Engine",
        windowWidth,
        windowHeight,
        SDL_WINDOW_OPENGL);

    if (!window) {
        // Handle window creation failure
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        // Handle OpenGL context creation failure
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        window = nullptr;
        return false;
    }

    

    return true; // Return true if initialization is successful
}

void Renderer::Draw() {
    // Rendering code here
}

void Renderer::Shutdown() {
    // Cleanup code here
    SDL_GL_DestroyContext(glContext);
    glContext = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
}