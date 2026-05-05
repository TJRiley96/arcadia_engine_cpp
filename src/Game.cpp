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
    Initialize();

    while (isRunning) {
        ProcessInput();
        Update();
        GenerateOutput();
    }

    Shutdown();
}


void Game::Initialize() {
    renderer = new Renderer(this);
    if (!renderer->Initialize(800.0f, 600.0f)) {
        // Handle renderer initialization failure
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize renderer");
        isRunning = false;
    } else {
        isRunning = true;
    }
}

void Game::Shutdown() {
    delete renderer;
    renderer = nullptr;
}

void Game::ProcessInput() {
    // Input processing code here
}

void Game::Update() {
    // Update game state here
}

void Game::GenerateOutput() {
    renderer->Draw();
}