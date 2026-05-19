#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "Component.h"

class SpriteComponent : public Component {
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* tex);

    // Getters
    int GetDrawOrder() const { return drawOrder; }
    class Texture* GetTexture() const { return texture; }
    int GetTexWidth() const { return texWidth; }
    int GetTexHeight() const { return texHeight; }

private:
    class Texture* texture;
    int drawOrder;
    int texWidth;
    int texHeight;
};