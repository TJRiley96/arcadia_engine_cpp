#pragma once

// Built-in includes
#include <cstddef>

// Third-party includes

// Local includes
#include "Component.h"

class MeshComponent: public Component {
public:
    MeshComponent(class Actor* owner);
    ~MeshComponent();

    virtual void Draw(class Shader* shader);

    // Getters
    const class Mesh* GetMesh() const { return mesh; }
    size_t GetTextureIndex() const { return textureIndex; }

    // Setters
    void SetMesh(class Mesh* m) { mesh = m; }
    void SetTextureIndex(size_t index) { textureIndex = index; }
private:
    class Mesh* mesh;
    size_t textureIndex;
};