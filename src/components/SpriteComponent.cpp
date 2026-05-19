#include "SpriteComponent.h"

// Built-in includes

// Third-party includes
#include <glad/glad.h>

// Local includes
#include "Actor.h"
#include "Game.h"
#include "Matrix.h"
#include "Texture.h"
#include "Shader.h"
#include "Renderer.h"

using Matrix::Matrix4;

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
    : Component(owner)
    , texture(nullptr)
    , drawOrder(drawOrder)
    , texWidth(0)
    , texHeight(0) {
    owner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
    if (texture) {
        texture->Unload();
    }
}

void SpriteComponent::SetTexture(Texture* tex) {
    texture = tex;
    if (texture) {
        texWidth = texture->GetWidth();
        texHeight = texture->GetHeight();
    } else {
        texWidth = 0;
        texHeight = 0;
    }
}

void SpriteComponent::Draw(Shader* shader) {
    if (texture) {
        // Set the texture on the shader
        Matrix4 scaleMat = Matrix4::Scaling(static_cast<float>(texWidth), static_cast<float>(texHeight), 1.0f);
        Matrix4 world = scaleMat * owner->GetWorldTransform();

        // Assuming the shader has a uniform called "uWorldTransform" for the world transform
        shader->SetUniformMatrix4fv("uWorldTransform", world);
        // Bind the texture
        texture->Bind();

        // Draw Quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}