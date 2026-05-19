#pragma once


// Built-in includes
#include <vector>

// Third-party includes

// Local includes
#include "Actor.h"
#include "Blocks.h"
#include "Config.h"
#include "Types.h"
#include "Vector.h"

using Vector::Vector3;
using Vector::Vector3i;


class Chunk: public Actor {
public:
    Chunk(class Game* game);
    ~Chunk();

    void UpdateActor(float deltaTime) override;
    void GenerateChunkMesh();
    void GenerateCubeMesh(Vector3i position);
    void RemoveCube(Vector3i position);
    void SetBlock(Vector3i position, BlockType type);


private:
    void InitBlockArrays();

    class MeshComponent* mesh;
    class ArrayMesh* arrayMesh;

    BlockType blocks[Config::chunk_size][Config::chunk_height][Config::chunk_size];

    bool updateMesh;

};