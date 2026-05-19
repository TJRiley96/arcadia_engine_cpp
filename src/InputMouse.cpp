#include "InputMouse.h"

// Built-in includes

// Third-party includes
#include <SDL3/SDL.h>

// Local includes


InputMouse::InputMouse()
    : x(0), y(0), leftButtonDown(false), rightButtonDown(false) {}

InputMouse::~InputMouse() {}

void InputMouse::Update() {
    float mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    x = mouseX;
    y = mouseY;
    leftButtonDown = (buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
    rightButtonDown = (buttons & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
}



