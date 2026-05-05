
#include "Renderer.h"
#include <glad/glad.h>

Renderer::Renderer(Game* game)
:game(game)
{
}

Renderer::~Renderer() {
    // Destructor code here
}

bool Renderer::Initialize(float width, float height) {
    windowWidth = width;
    windowHeight = height;

    state.window = SDL_CreateWindow(
        "Arcadia Engine",
        width,
        height,
        0);

    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Creation Error", SDL_GetError(), nullptr);
        Shutdown();
        return false;
    }

    return true; // Return true if initialization is successful
}

void Renderer::Draw() {
    // Rendering code here
}

void Renderer::Shutdown() {
    // Cleanup code here
    // SDL_GL_DestroyContext(state.glContext);
    // state.glContext = nullptr;
    if (state.window) {
        SDL_DestroyWindow(state.window);
        state.window = nullptr;
    }
    SDL_Quit();
}