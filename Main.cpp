// Main.cpp

// Built-in includes

// Third-party includes
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// Local includes
#include "Logging.h"
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

    Logging::SanityCheck();

    Game game;
    if (game.Initialize()) {
        game.Run();
    }
    return 0;
}
