#include <SDL3/SDL.h>



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

    SDL_Window* window;
    SDL_GLContext glContext;

    class Game* game; // Forward declaration of Game class

};