#pragma once

// Built-in includes
#include <string>
#include <vector>

// Third-party includes

// Local includes
#include "Types.h"

enum PrimitiveShape {
    Cube,
    Sphere,
    Plane
};

enum ArrayType{
    ArrayVertex,
    ArrayNormal,
    ArrayUV,
    ArrayIndex
};

class Mesh{
public:
    Mesh();
    ~Mesh();

    virtual void Load(const std::string& fileName, class Renderer* renderer);
    virtual void Load(PrimitiveShape shape, class Renderer* renderer);
    virtual void Load(const std::vector<Types::Vertex8>& vertices, const std::vector<Types::Index>& indices, class Renderer* renderer);
    virtual void LoadRawMesh(const std::vector<Types::Vertex3>& vertices, const std::vector<Types::UV>& uvs, const std::vector<Types::Index>& indices, const std::vector<class Texture*>& textures, class Renderer* renderer);
    virtual void LoadRawMesh(const std::vector<Types::Vertex3>& vertices, const std::vector<Types::UV>& uvs, const std::vector<Types::Vertex3>& normals, const std::vector<Types::Index>& indices, const std::vector<class Texture*>& textures, class Renderer* renderer);
    virtual void Unload();

    // Getters
    class Texture* GetTexture(size_t index) const { return textures[index]; }
    size_t GetTextureCount() const { return textures.size(); }
    class VertexArray* GetVertexArray() const { return vao; }
    const std::string& GetShaderName() const { return shaderName; }
    float GetRadius() const { return radius; }
    float GetSpecPower() const { return specPower; };

    // setters

    void SetShaderName(const std::string& name) { shaderName = name; }
    void SetRadius(float r) { radius = r; }
    void SetSpecPower(float sp) { specPower = sp; }


private:
    std::vector<class Texture*> textures;

    // Vertex Array Object
    class VertexArray* vao;

    std::string shaderName;

    float radius;

    float specPower;

    bool LoadGPMesh(const std::string& fileName, class Renderer* renderer);
    bool LoadOBJ(const std::string& fileName, class Renderer* renderer);

};