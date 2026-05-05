#include <SDL3/SDL.h>

struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

class Renderer {
public:
    Renderer(class Game* game);
    ~Renderer();

    void Draw();

    bool Initialize(float width, float height);
    void Shutdown();

private:
    // Private member variables and functions here

    void CreateSpriteVerts();

    float windowWidth;
    float windowHeight;

    SDLState state;

    class Game* game; // Forward declaration of Game class

};