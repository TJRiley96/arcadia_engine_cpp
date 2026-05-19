#include "Renderer.h"

// Built-in includes
#include <algorithm>

// Third-party includes

// Local includes
#include "Color.h"
#include "Config.h"
#include "Game.h"
#include "Logging.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Shader.h"

#include "VertexArray.h"

#include "Texture.h"
#include "SpriteComponent.h"


Renderer::Renderer(Game* game)
:game(game), clearColor("#096470")
{
}

Renderer::~Renderer() {
    // Destructor code here
}

bool Renderer::Initialize(float width, float height) {
    windowWidth = width;
    windowHeight = height;

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Request an OpenGL 3.3 context (should be supported on most platforms)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Request hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    state.window = SDL_CreateWindow(
        "Arcadia Engine",
        static_cast<int>(width),
        static_cast<int>(height),
        SDL_WINDOW_OPENGL);

    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Creation Error", SDL_GetError(), nullptr);
        Shutdown();
        return false;
    }
    Logging::LogInfo("Window created successfully");

    state.glContext = SDL_GL_CreateContext(state.window);

    if (!state.glContext) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Context Creation Error", SDL_GetError(), nullptr);
        Shutdown();
        return false;
    }
    Logging::LogInfo("OpenGL context created successfully");

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OpenGL Initialization Error", "Failed to initialize OpenGL context", nullptr);
        Shutdown();
        return false;
    }
    Logging::LogInfo("OpenGL initialized successfully");

    glViewport(0, 0, windowWidth, windowHeight);

    if (!LoadShaders()) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Loading Error", "Failed to load shaders", nullptr);
        Shutdown();
        return false;
    }

    Logging::LogInfo("Shaders loaded successfully");

    CreateSpriteVerts();




    return true; // Return true if initialization is successful
}

void Renderer::Draw() {
    // Rendering code here
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    meshShader->Use();

    meshShader->SetUniformMatrix4fv("uViewProj", projection * view);
    for (MeshComponent* meshComp : meshComps) {
        meshComp->Draw(meshShader);
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    spriteShader->Use();
    spriteVerts->Bind();
    for (SpriteComponent* sprite : sprites) {
        sprite->Draw(spriteShader);
    }

    SDL_GL_SwapWindow(state.window);
}

void Renderer::Shutdown() {
    // Cleanup code here
    if (state.glContext) {
        SDL_GL_DestroyContext(state.glContext);
        state.glContext = nullptr;
    }
    if (state.window) {
        SDL_DestroyWindow(state.window);
        state.window = nullptr;
    }
    SDL_Quit();
    Logging::LogInfo("Renderer shutdown successfully");
}

void Renderer::CreateSpriteVerts() {
    // Code to create vertex data for sprites

    spriteVerts = new VertexArray(Config::QUAD_8, 4, Config::QUAD_I, 2);
}

bool Renderer::LoadShaders() {
    spriteShader = new Shader();
    if (!spriteShader->Load(Config::vertShaderPath, Config::fragShaderPath)) {
        Logging::LogError("Failed to load sprite shader");
        return false;
    }

    spriteShader->Use();

    Matrix4 viewProj = Matrix4::CreateSimpleView(windowWidth, windowHeight);
    spriteShader->SetUniformMatrix4fv("uViewProj", viewProj);

    meshShader = new Shader();
    if (!meshShader->Load(Config::meshVertShaderPath, Config::meshFragShaderPath)) {
        Logging::LogError("Failed to load mesh shader");
        return false;
    }
    view = Matrix4::CreateLookAt(Vector3(0.0f, -900.0f, 420.0f), Vector3(0.0f, 0.0f, 100.0f), Vector3::Up);
    projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(55.0f), windowWidth, windowHeight, 25.0f, 5000.0f);

    meshShader->SetUniformMatrix4fv("uViewProj", projection * view);

    return true;
}

void Renderer::UnloadShaders() {
    if (spriteShader) {
        spriteShader->Unload();
        delete spriteShader;
        spriteShader = nullptr;
    }
}

void Renderer::UnloadData() {
    UnloadShaders();

    if (spriteVerts) {
        delete spriteVerts;
        spriteVerts = nullptr;
    }

    for (SpriteComponent* sprite : sprites) {
        if (sprite) {
            delete sprite;
        }
    }
    sprites.clear();

    for (auto& pair : textures) {
        if (pair.second) {
            pair.second->Unload();
            delete pair.second;
        }
    }
    textures.clear();
}

void Renderer::AddSprite(SpriteComponent* sprite) {
    sprites.push_back(sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
    auto iter = std::find(sprites.begin(), sprites.end(), sprite);
    if (iter != sprites.end()) {
        sprites.erase(iter);
    }
}

void Renderer::RemoveMeshComponent(MeshComponent* meshComp) {
    auto iter = std::find(meshComps.begin(), meshComps.end(), meshComp);
    if (iter != meshComps.end()) {
        meshComps.erase(iter);
    }
}

Texture* Renderer::GetTexture(const std::string& name) {
    // Code to load and return a texture by name
    // Check if texture is already loaded
    Texture* tex = nullptr;
	auto iter = textures.find(name);
	if (iter != textures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(name.c_str(), FilterMode::NEAREST))
		{
			textures.emplace(name, tex);
            std::string msg = "Texture loaded and added to renderer: " + name;
            Logging::LogInfo(msg.c_str());
		}
		else
		{
			std::string msg = "Failed to load texture: " + name;
			Logging::LogError(msg.c_str());
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string& name) {
    // Code to load and return a mesh by name
    // Check if mesh is already loaded
    Mesh* mesh = nullptr;
    auto iter = meshes.find(name);
    if (iter != meshes.end())
    {
        mesh = iter->second;
    }
    else
    {
        mesh = new Mesh();
        mesh->Load(name, this);
        if(mesh->GetVertexArray()) {
            meshes.emplace(name, mesh);
            std::string msg = "Mesh loaded and added to renderer: " + name;
            Logging::LogInfo(msg.c_str());
        } else {
            std::string msg = "Failed to load mesh: " + name;
            Logging::LogError(msg.c_str());
            delete mesh;
            mesh = nullptr;
        }
    }
    return mesh;
}