#include "Chunk.h"

// Built-in includes

// Third-party includes

// Local includes
#include "ArrayMesh.h"
#include "Config.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Texture.h"


Chunk::Chunk(Game* game)
    : Actor(game), mesh(nullptr), arrayMesh(nullptr), updateMesh(true) {
    InitBlockArrays();

    mesh = new MeshComponent(this);
    arrayMesh = new ArrayMesh(Config::chunk_size, Config::chunk_height, Config::chunk_size);
    arrayMesh->SetTextureGridSize(Config::texture_grid_size);
    arrayMesh->SetTexture(GetGame()->GetRenderer()->GetTexture(Config::TEST_CUBE_PNG));
    mesh->SetMesh(arrayMesh);

}

Chunk::~Chunk() {
    if (mesh) {
        delete mesh;
        mesh = nullptr;
    }
    if (arrayMesh) {
        delete arrayMesh;
        arrayMesh = nullptr;
    }
}

void Chunk::UpdateActor(float deltaTime) {
    if (updateMesh) {
        GenerateChunkMesh();
        updateMesh = false;
    }
}

void Chunk::GenerateChunkMesh() {
    if (!arrayMesh) {
        return;
    }

    arrayMesh->Fill(BlockType::Air);

    for (unsigned int x = 0; x < Config::chunk_size; x++) {
        for (unsigned int y = 0; y < Config::chunk_height; y++) {
            for (unsigned int z = 0; z < Config::chunk_size; z++) {
                if (blocks[x][y][z] != BlockType::Air) {
                    arrayMesh->SetBlock(Vector3i(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)), blocks[x][y][z]);
                }
            }
        }
    }

    arrayMesh->Rebuild(GetGame()->GetRenderer());
}

void Chunk::GenerateCubeMesh(Vector3i pos) {
    if (!arrayMesh || !arrayMesh->InBounds(pos)) {
        return;
    }

    SetBlock(pos, BlockType::Dirt);
}

void Chunk::RemoveCube(Vector3i pos) {
    if (!arrayMesh || !arrayMesh->InBounds(pos)) {
        return;
    }

    SetBlock(pos, BlockType::Air);
}

void Chunk::SetBlock(Vector3i pos, BlockType type) {
    if (!arrayMesh || !arrayMesh->InBounds(pos)) {
        return;
    }

    blocks[pos.x][pos.y][pos.z] = type;
    arrayMesh->SetBlock(pos, type);
    updateMesh = true;
}

void Chunk::InitBlockArrays() {
    for (unsigned int x = 0; x < Config::chunk_size; ++x) {
        for (unsigned int y = 0; y < Config::chunk_height; ++y) {
            for (unsigned int z = 0; z < Config::chunk_size; ++z) {
                blocks[x][y][z] = BlockType::Dirt; // Initialize all blocks to dirt
            }
        }
    }
}