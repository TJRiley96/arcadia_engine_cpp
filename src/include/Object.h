#pragma once

// Built-in includes
#include <cmath>
#include <stdint.h>
#include <vector>

// Third-party includes

// Local includes


class Object {
public:
    Object();
    ~Object();

    void Load(const std::string& fileName, class Renderer* renderer);
    void Unload();

    // Getters for object properties
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
    class VertexArray* vao;
    std::string shaderName;
    float radius;
    float specPower;
};
