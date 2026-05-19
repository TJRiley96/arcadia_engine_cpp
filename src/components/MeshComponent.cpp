#include "MeshComponent.h"

// Built-in includes

// Third-party includes

// Local includes
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Logging.h"

MeshComponent::MeshComponent(class Actor* owner)
    : Component(owner), mesh(nullptr), textureIndex(0) {
        owner->GetGame()->GetRenderer()->AddMeshComponent(this);
    }

MeshComponent::~MeshComponent() {
    owner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
}

void MeshComponent::Draw(Shader* shader) {
    if (mesh) {
        VertexArray* vao = mesh->GetVertexArray();
        if (!vao) {
            return;
        }

        shader->SetUniformMatrix4fv("uWorldTransform", owner->GetWorldTransform());
        shader->SetUniform1i("uTexture", 0);
        // shader->SetUniform1f("uRadius", mesh->GetRadius());
        // shader->SetUniform1f("uSpecPower", mesh->GetSpecPower());
        Texture* tex = nullptr;
        if (mesh->GetTextureCount() > textureIndex) {
            tex = mesh->GetTexture(textureIndex);
        }

        // Set any necessary uniforms here, such as transformation matrices
        if (tex) {
            tex->Bind();
        }
        vao->Bind();

        glDrawElements(GL_TRIANGLES, vao->GetIndexCount(), GL_UNSIGNED_INT, 0);
    }
}