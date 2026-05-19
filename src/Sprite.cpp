#include "Sprite.h"

// Built-in includes

// Third-party includes
#include <SDL3/SDL.h>

// Local includes
#include "Actor.h"
#include "Config.h"
#include "Game.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "InputComponent.h"

Sprite::Sprite(Game* game)
: Actor(game) {

    // create sprite-specific components here (e.g., SpriteComponent)
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetRenderer()->GetTexture(Config::DEFAULT_TEXTURE));

    InputComponent* ic = new InputComponent(this);
    ic->SetMaxForwardSpeed(20.0f);
    ic->SetMaxTurnSpeed(Math::ToRadians(360.0f));
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackwardKey(SDL_SCANCODE_S);
    ic->SetRotateLeftKey(SDL_SCANCODE_Q);
    ic->SetRotateRightKey(SDL_SCANCODE_E);
}

void Sprite::UpdateActor(float deltaTime) {
    Actor::UpdateActor(deltaTime);
    // Sprite-specific update code here
}

void Sprite::ActorInput(const bool* state) {
    Actor::ActorInput(state);
    // Sprite-specific input processing here

}
