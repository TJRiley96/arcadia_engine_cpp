#pragma once

// Built-in includes
#include <vector>

// Third-party includes

// Local includes
#include "Blocks.h"
#include "Config.h"
#include "Mesh.h"
#include "Vector.h"

class ArrayMesh : public Mesh {
public:
    ArrayMesh(unsigned int width = Config::chunk_size,
        unsigned int height = Config::chunk_height,
        unsigned int depth = Config::chunk_size);
    ~ArrayMesh();

    void Load(const std::string& fileName, class Renderer* renderer) override;
    void Unload() override;

    void SetTexture(class Texture* texture);
    void SetTextureGridSize(unsigned int gridSize);

    bool InBounds(const Vector::Vector3i& pos) const;
    void SetBlock(const Vector::Vector3i& pos, BlockType type);
    BlockType GetBlock(const Vector::Vector3i& pos) const;
    void Fill(BlockType type);

    void Rebuild(class Renderer* renderer);

private:
    size_t BlockIndex(unsigned int x, unsigned int y, unsigned int z) const;
    bool IsSolid(int x, int y, int z) const;

    void AppendFace(const Types::Vertex3& v0, const Types::Vertex3& v1,
        const Types::Vertex3& v2, const Types::Vertex3& v3,
        float nx, float ny, float nz,
        unsigned int tileX, unsigned int tileY,
        std::vector<Types::Vertex3>& outVertices,
        std::vector<Types::UV>& outUVs,
        std::vector<Types::Normal>& outNormals,
        std::vector<Types::Index>& outIndices) const;

    unsigned int width;
    unsigned int height;
    unsigned int depth;

    unsigned int textureGridSize;
    float tileStep;

    std::vector<BlockType> blocks;
    std::vector<class Texture*> meshTextures;

};
