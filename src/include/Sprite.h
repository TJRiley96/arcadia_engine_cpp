#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "Actor.h"


class Sprite: public Actor {
public:
    Sprite(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const bool* state) override;

};
