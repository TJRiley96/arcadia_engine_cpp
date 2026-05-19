#include "ArrayMesh.h"

// Built-in includes
#include <algorithm>

// Third-party includes

// Local includes
#include "Renderer.h"
#include "Texture.h"


ArrayMesh::ArrayMesh(unsigned int width, unsigned int height, unsigned int depth)
    : Mesh(),
    width(width),
    height(height),
    depth(depth),
    textureGridSize(Config::texture_grid_size),
    tileStep(1.0f / static_cast<float>(Config::texture_grid_size)) {
    blocks.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(depth), BlockType::Air);
}

ArrayMesh::~ArrayMesh() {
    Unload();
}

void ArrayMesh::Load(const std::string& fileName, Renderer* renderer) {
    Mesh::Load(fileName, renderer);
}

void ArrayMesh::Unload() {
    Mesh::Unload();
    std::fill(blocks.begin(), blocks.end(), BlockType::Air);
}

void ArrayMesh::SetTexture(Texture* texture) {
    meshTextures.clear();
    if (texture) {
        meshTextures.push_back(texture);
    }
}

void ArrayMesh::SetTextureGridSize(unsigned int gridSize) {
    if (gridSize == 0) {
        return;
    }
    textureGridSize = gridSize;
    tileStep = 1.0f / static_cast<float>(textureGridSize);
}

bool ArrayMesh::InBounds(const Vector::Vector3i& pos) const {
    return pos.x >= 0 && pos.y >= 0 && pos.z >= 0
        && pos.x < static_cast<int>(width)
        && pos.y < static_cast<int>(height)
        && pos.z < static_cast<int>(depth);
}

void ArrayMesh::SetBlock(const Vector::Vector3i& pos, BlockType type) {
    if (!InBounds(pos)) {
        return;
    }

    blocks[BlockIndex(static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y), static_cast<unsigned int>(pos.z))] = type;
}

BlockType ArrayMesh::GetBlock(const Vector::Vector3i& pos) const {
    if (!InBounds(pos)) {
        return BlockType::Air;
    }

    return blocks[BlockIndex(static_cast<unsigned int>(pos.x), static_cast<unsigned int>(pos.y), static_cast<unsigned int>(pos.z))];
}

void ArrayMesh::Fill(BlockType type) {
    std::fill(blocks.begin(), blocks.end(), type);
}

void ArrayMesh::Rebuild(Renderer* renderer) {
    std::vector<Types::Vertex3> vertices;
    std::vector<Types::UV> uvs;
    std::vector<Types::Normal> normals;
    std::vector<Types::Index> indices;

    vertices.reserve(blocks.size() * 4);
    uvs.reserve(blocks.size() * 4);
    normals.reserve(blocks.size() * 4);
    indices.reserve(blocks.size() * 2);

    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int z = 0; z < depth; ++z) {
                const Vector::Vector3i pos(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z));
                if (GetBlock(pos) == BlockType::Air) {
                    continue;
                }

                const float fx = static_cast<float>(x);
                const float fy = static_cast<float>(y);
                const float fz = static_cast<float>(z);

                if (!IsSolid(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z + 1))) {
                    AppendFace({-0.5f + fx, -0.5f + fy, 0.5f + fz},
                        {0.5f + fx, -0.5f + fy, 0.5f + fz},
                        {0.5f + fx, 0.5f + fy, 0.5f + fz},
                        {-0.5f + fx, 0.5f + fy, 0.5f + fz},
                        0.0f, 0.0f, 1.0f,
                        0, 0,
                        vertices, uvs, normals, indices);
                }

                if (!IsSolid(static_cast<int>(x + 1), static_cast<int>(y), static_cast<int>(z))) {
                    AppendFace({0.5f + fx, -0.5f + fy, -0.5f + fz},
                        {0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {0.5f + fx, 0.5f + fy, 0.5f + fz},
                        {0.5f + fx, -0.5f + fy, 0.5f + fz},
                        1.0f, 0.0f, 0.0f,
                        3, 0,
                        vertices, uvs, normals, indices);
                }

                if (!IsSolid(static_cast<int>(x), static_cast<int>(y - 1), static_cast<int>(z))) {
                    AppendFace({-0.5f + fx, -0.5f + fy, -0.5f + fz},
                        {0.5f + fx, -0.5f + fy, -0.5f + fz},
                        {0.5f + fx, -0.5f + fy, 0.5f + fz},
                        {-0.5f + fx, -0.5f + fy, 0.5f + fz},
                        0.0f, -1.0f, 0.0f,
                        0, 1,
                        vertices, uvs, normals, indices);
                }

                if (!IsSolid(static_cast<int>(x - 1), static_cast<int>(y), static_cast<int>(z))) {
                    AppendFace({-0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {-0.5f + fx, -0.5f + fy, -0.5f + fz},
                        {-0.5f + fx, -0.5f + fy, 0.5f + fz},
                        {-0.5f + fx, 0.5f + fy, 0.5f + fz},
                        -1.0f, 0.0f, 0.0f,
                        1, 1,
                        vertices, uvs, normals, indices);
                }

                if (!IsSolid(static_cast<int>(x), static_cast<int>(y + 1), static_cast<int>(z))) {
                    AppendFace({0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {-0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {-0.5f + fx, 0.5f + fy, 0.5f + fz},
                        {0.5f + fx, 0.5f + fy, 0.5f + fz},
                        0.0f, 1.0f, 0.0f,
                        2, 0,
                        vertices, uvs, normals, indices);
                }

                if (!IsSolid(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z - 1))) {
                    AppendFace({-0.5f + fx, -0.5f + fy, -0.5f + fz},
                        {-0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {0.5f + fx, 0.5f + fy, -0.5f + fz},
                        {0.5f + fx, -0.5f + fy, -0.5f + fz},
                        0.0f, 0.0f, -1.0f,
                        1, 0,
                        vertices, uvs, normals, indices);
                }
            }
        }
    }

    if (vertices.empty() || indices.empty()) {
        Unload();
        return;
    }

    Mesh::LoadRawMesh(vertices, uvs, normals, indices, meshTextures, renderer);
}

size_t ArrayMesh::BlockIndex(unsigned int x, unsigned int y, unsigned int z) const {
    return (static_cast<size_t>(x) * static_cast<size_t>(height) + static_cast<size_t>(y)) * static_cast<size_t>(depth) + static_cast<size_t>(z);
}

bool ArrayMesh::IsSolid(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0
        || x >= static_cast<int>(width)
        || y >= static_cast<int>(height)
        || z >= static_cast<int>(depth)) {
        return false;
    }

    return blocks[BlockIndex(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z))] != BlockType::Air;
}

void ArrayMesh::AppendFace(const Types::Vertex3& v0, const Types::Vertex3& v1,
    const Types::Vertex3& v2, const Types::Vertex3& v3,
    float nx, float ny, float nz,
    unsigned int tileX, unsigned int tileY,
    std::vector<Types::Vertex3>& outVertices,
    std::vector<Types::UV>& outUVs,
    std::vector<Types::Normal>& outNormals,
    std::vector<Types::Index>& outIndices) const {
    const unsigned int base = static_cast<unsigned int>(outVertices.size());

    outVertices.push_back(v0);
    outVertices.push_back(v1);
    outVertices.push_back(v2);
    outVertices.push_back(v3);

    outNormals.push_back({nx, ny, nz});
    outNormals.push_back({nx, ny, nz});
    outNormals.push_back({nx, ny, nz});
    outNormals.push_back({nx, ny, nz});

    const float u = static_cast<float>(tileX) * tileStep;
    const float v = static_cast<float>(tileY) * tileStep;
    outUVs.push_back({u, v + tileStep});
    outUVs.push_back({u + tileStep, v + tileStep});
    outUVs.push_back({u + tileStep, v});
    outUVs.push_back({u, v});

    outIndices.push_back({base + 0, base + 1, base + 2});
    outIndices.push_back({base + 2, base + 3, base + 0});
}