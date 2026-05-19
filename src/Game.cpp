#include "Game.h"

// Built-in includes
#include <algorithm>
#include <cmath>
#include <iostream>

// Third-party includes

// Local includes
#include "Actor.h"
#include "Camera.h"
#include "Chunk.h"
#include "Config.h"
#include "Logging.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Sprite.h"
#include "Quaternion.h"
#include "Renderer.h"
#include "PlaneActor.h"
#include "Texture.h"
#include "Shader.h"

Game::Game()
: ticksCount(0),
isRunning(false),
renderer(nullptr),
camera(nullptr),
chunk(nullptr),
placeBlockKeyDown(false),
removeBlockKeyDown(false),
updatingActors(false){
    // Constructor code here
}

Game::~Game() {
    // Destructor code here
}

void Game::Run() {

    while (isRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }

    Shutdown();
}


bool Game::Initialize() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization Error", SDL_GetError(), nullptr);
        return false;
    }

    //std::cout << "SDL initialized successfully." << std::endl;
    Logging::LogInfo("SDL initialized successfully.");

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

    LoadData();

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

    if (camera) {
        camera->ProcessInput();
    }

    if (camera && chunk) {
        const bool placePressed = keyState[SDL_SCANCODE_E];
        const bool removePressed = keyState[SDL_SCANCODE_Q];

        if ((placePressed && !placeBlockKeyDown) || (removePressed && !removeBlockKeyDown)) {
            const Vector3 samplePoint = camera->GetPosition() + camera->GetForward() * 120.0f;
            const float voxelWorldSize = chunk->GetScale().x;
            Vector3i blockPos(
                static_cast<int>(std::floor(samplePoint.x / voxelWorldSize + 0.5f)),
                static_cast<int>(std::floor(samplePoint.y / voxelWorldSize + 0.5f)),
                static_cast<int>(std::floor(samplePoint.z / voxelWorldSize + 0.5f)));

            if (placePressed && !placeBlockKeyDown) {
                chunk->GenerateCubeMesh(blockPos);
            }
            if (removePressed && !removeBlockKeyDown) {
                chunk->RemoveCube(blockPos);
            }
        }

        placeBlockKeyDown = placePressed;
        removeBlockKeyDown = removePressed;
    }

    if (keyState[SDL_SCANCODE_F5]) {
        renderer->SetPolygonMode(RenderMode::Fill);
    } else if (keyState[SDL_SCANCODE_F6]) {
        renderer->SetPolygonMode(RenderMode::Wireframe);
    } else if (keyState[SDL_SCANCODE_F7]) {
        renderer->SetPolygonMode(RenderMode::Point);
    }
}

void Game::UpdateGame() {
    // Update game state here
    unsigned int currentTicks = SDL_GetTicks();
    float deltaTime = (currentTicks - ticksCount) / 1000.0f;
    ticksCount = currentTicks;

    updatingActors = true;
    for (auto actor : actors) {
        actor->Update(deltaTime);
    }
    updatingActors = false;

    // Move any pending actors to the main actors vector
   for (auto pendingActor : pendingActors) {
        pendingActor->ComputeWorldTransform(); // Ensure world transform is computed before adding to main actors list
        actors.emplace_back(pendingActor);
    }
    pendingActors.clear();


    // Add any dead actors to a temporary vector for deletion after the loop
    std::vector<Actor*> deadActors;
    for (auto actor : actors) {
        if (actor->GetState() == Actor::Dead) {
            deadActors.push_back(actor);
        }
    }

    // Remove dead actors from the main actors vector and delete them
    for (auto deadActor : deadActors) {
        RemoveActor(deadActor);
        delete deadActor;
    }

}

void Game::GenerateOutput() {
    renderer->Draw();
}

void Game::LoadData() {
    renderer->GetTexture(Config::DEFAULT_TEXTURE);

    // Load game data here
    Actor* temp = new Actor(this);

    temp->SetPosition(Vector3(0.0f, 0.0f, 100.0f));
    temp->SetScale(100.0f);
    temp->SetRotation(Math::ToRadians(50.0f));

    MeshComponent* mc = new MeshComponent(temp);
    mc->SetMesh(renderer->GetMesh("assets/obj/cube.gpmesh"));

    camera = new Camera(this);

    // Create a plane actor

    // const int gridCount = 10;
    // const float tileWorldSize = 250.0f;
    // const float tileScale = tileWorldSize / 100.0f; // Plane.gpmesh spans 100 units.
    // const float start = -((gridCount - 1) * tileWorldSize * 0.5f);
    // for (int i = 0; i < gridCount; i++) {
    //     for (int j = 0; j < gridCount; j++) {
    //         temp = new PlaneActor(this);
    //         temp->SetPosition(Vector3(start + i * tileWorldSize, start + j * tileWorldSize, 0.0f));
    //         temp->SetScale(tileScale);
    //     }
    // }

    chunk = new Chunk(this);
    chunk->SetScale(100.0f);
    chunk->SetPosition(Vector3(0.0f, 0.0f, 0.0f));




}

void Game::UnloadData() {
    // Unload game data here
    if (camera) {
        delete camera;
        camera = nullptr;
    }
}


void Game::AddActor(Actor* actor) {
    if (updatingActors) {
        pendingActors.push_back(actor);
    } else {
        actors.push_back(actor);
    }
}

void Game::RemoveActor(Actor* actor) {
    // Check if actor is in pendingActors
    auto pendingIt = std::find(pendingActors.begin(), pendingActors.end(), actor);
    if (pendingIt != pendingActors.end()) {
        pendingActors.erase(pendingIt);
        return;
    }

    // Check if actor is in actors
    auto it = std::find(actors.begin(), actors.end(), actor);
    if (it != actors.end()) {
        actors.erase(it);
    }
}