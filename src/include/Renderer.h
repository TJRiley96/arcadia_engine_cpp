#pragma once

// Built-in includes
#include <unordered_map>
#include <vector>

// Third-party includes
#include <glad/glad.h>
#include <SDL3/SDL.h>

// Local includes
#include "Color.h"
#include "Matrix.h"

using Matrix::Matrix4;

enum RenderMode {
    Wireframe = GL_LINE,
    Fill = GL_FILL,
    Point = GL_POINT
};

struct SDLState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext glContext;
};

class Renderer {
public:
    Renderer(class Game* game);
    ~Renderer();

    bool Initialize(float width, float height);
    void Shutdown();
    void UnloadData();

    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    void AddMeshComponent(class MeshComponent* meshComp) { meshComps.push_back(meshComp); }
    void RemoveMeshComponent(class MeshComponent* meshComp);

    class Texture* GetTexture(const std::string& name);
    class Mesh* GetMesh(const std::string& name);

    void SetViewMatrix(const Matrix4& view) { this->view = view; }

    class Shader* GetSpriteShader() { return spriteShader; }

    void SetClearColor(const Color::Color4& color) {
        clearColor = color;
    }

    void SetPolygonMode(RenderMode mode) {
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }

    float GetWindowWidth() const { return windowWidth; }
    float GetWindowHeight() const { return windowHeight; }

private:
    // Private member variables and functions here

    void CreateSpriteVerts();

    bool LoadShaders();
    void UnloadShaders();

    // Map of texture slots to texture IDs
    std::unordered_map<std::string, class Texture*> textures;
    // Map of meshes
    std::unordered_map<std::string, class Mesh*> meshes;

    // All sprite components drawn
    std::vector<class SpriteComponent*> sprites;

    // All mesh components drawn
    std::vector<class MeshComponent*> meshComps;

    class Game* game; // Forward declaration of Game class

    float windowWidth;
    float windowHeight;

    // Shaders and vertex arrays
    class Shader* spriteShader;
    class Shader* meshShader;
    // class Shader* meshShader;
    class VertexArray* spriteVerts;

    // view and projection matrices
    Matrix4 view;
    Matrix4 projection;

    SDLState state;

    Color::Color4 clearColor;

};