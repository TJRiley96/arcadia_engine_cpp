#include <iostream>
#include "Game.h"
#include "Renderer.h"


Game::Game()
: ticksCount(0),
isRunning(false),
renderer(nullptr) {
    // Constructor code here
}

Game::~Game() {
    // Destructor code here
}

void Game::Run() {

    while (isRunning) {
        ProcessInput();
        Update();
        GenerateOutput();
    }

    Shutdown();
}


bool Game::Initialize() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization Error", SDL_GetError(), nullptr);
        return false;
    }

    std::cout << "SDL initialized successfully." << std::endl;

    renderer = new Renderer(this);
    if (!renderer->Initialize(800.0f, 800.0f)) {
        // Handle renderer initialization failure
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer Initialization Error", "Failed to initialize renderer", nullptr);
        isRunning = false;
        return false;
    } else {
        isRunning = true;
    }

    ticksCount = SDL_GetTicks();

    return true; // Return true if initialization is successful
}

void Game::Shutdown() {
    if(renderer) {
        renderer->Shutdown();
        delete renderer;
        renderer = nullptr;
    }
    SDL_Quit();
}

void Game::ProcessInput() {
    // Input processing code here
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            // Handle other events here
        }
    }
    const bool* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }
}

void Game::Update() {
    // Update game state here
}

void Game::GenerateOutput() {
    renderer->Draw();
}