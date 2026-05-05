#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Game.h"
#include "Renderer.h"

void cleanup(SDLState &state) {
    if (state.window) {
        SDL_DestroyWindow(state.window);
        state.window = nullptr;
    }
    SDL_Quit();
    // if (state.glContext) {
    //     SDL_GL_DestroyContext(state.glContext);
    //     state.glContext = nullptr;
    // }
}

int main(int argc, char* argv[]) {

    // SDLState state;

    // if (!SDL_Init(SDL_INIT_VIDEO)) {
    //     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization Error", SDL_GetError(), nullptr);
    //     return 1;
    // }

    // int width = 800;
    // int height = 600;

    // state.window = SDL_CreateWindow(
    //     "Arcadia Engine",
    //     width,
    //     height,
    //     0);

    // if (!state.window) {
    //     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Creation Error", SDL_GetError(), nullptr);
    //     cleanup(state);
    //     return 1;
    // }

    // bool running = true;
    // while (running) {
    //     SDL_Event event;
    //     while (SDL_PollEvent(&event)) {
    //         switch (event.type)
    //         {
    //         case SDL_EVENT_QUIT:
    //             running = false;
    //             break;

    //         default:
    //             break;
    //         }

    //     }

    //     // Rendering code here
    // }

    // cleanup(state);
    // return 0;

    Game game;
    if (game.Initialize()) {
        game.Run();
    } else {
        // Handle initialization failure if necessary
        return 1;
    }
}