#pragma once

// Built-in includes
#include <vector>

// Third-party includes
#include <SDL3/SDL.h>

// Local includes



class Game {
public:
    Game();
    ~Game();

    void Run();
    bool Initialize();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    class Renderer* GetRenderer() { return renderer; }

private:

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    void LoadData();
    void UnloadData();

    std::vector<class Actor*> actors;

    std::vector<class Actor*> pendingActors;

    unsigned int ticksCount;

    bool isRunning;
    bool updatingActors;

    class Renderer* renderer; // Forward declaration of Renderer class

    // For the project
    class Camera* camera;
    class Chunk* chunk;

    bool placeBlockKeyDown;
    bool removeBlockKeyDown;

};
