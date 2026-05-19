#include "InputComponent.h"

// Built-in includes

// Third-party includes
#include <SDL3/SDL.h>

// Local includes
#include "Actor.h"
#include "Logging.h"
#include "Types.h"

using Types::KeyCode;

InputComponent::InputComponent(Actor* owner, int updateOrder)
: MoveComponent(owner, updateOrder),
  maxForwardSpeed(0.0f),
  maxTurnSpeed(0.0f),
  forwardKey(SDL_SCANCODE_UP),
  backwardKey(SDL_SCANCODE_DOWN),
  rotateLeftKey(SDL_SCANCODE_LEFT),
  rotateRightKey(SDL_SCANCODE_RIGHT) {}


void InputComponent::ProcessInput() {
    const bool* keyState = SDL_GetKeyboardState(NULL);
    Vector3 moveSpeed = Vector3::Zero;
    float turnSpeed = 0.0f;

    if (keyState[forwardKey]) {
        moveSpeed += Vector3::Forward * maxForwardSpeed;
        //Logging::LogInfo("Forward key pressed.");
    }
    if (keyState[backwardKey]) {
        moveSpeed -= Vector3::Forward * maxForwardSpeed;
        //Logging::LogInfo("Backward key pressed.");
    }
    SetMoveSpeed(moveSpeed);

    if (keyState[rotateLeftKey]) {
        turnSpeed += maxTurnSpeed;
        //Logging::LogInfo("Rotate left key pressed.");
    }
    if (keyState[rotateRightKey]) {
        turnSpeed -= maxTurnSpeed;
        //Logging::LogInfo("Rotate right key pressed.");
    }
    SetAngularSpeed(turnSpeed);
}