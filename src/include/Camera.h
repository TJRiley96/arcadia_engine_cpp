#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "Actor.h"

class Camera: public Actor {
public:
    Camera(class Game* game);

    // Update functions
    void UpdateActor(float deltaTime) override;

    // Input processing
    void ActorInput(const bool* state) override;

private:
    class MoveComponent* mc;
};