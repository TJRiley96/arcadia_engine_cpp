#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "MoveComponent.h"
#include "Types.h"
#include "Vector.h"

using Vector::Vector3;

using Types::KeyCode;

class InputComponent: public MoveComponent {
public:
    InputComponent(class Actor* owner, int updateOrder = 100);

    void ProcessInput() override;

    // Getters
    float GetMaxForwardSpeed() const { return maxForwardSpeed; }
    float GetMaxTurnSpeed() const { return maxTurnSpeed; }
    KeyCode GetForwardKey() const { return forwardKey; }
    KeyCode GetBackwardKey() const { return backwardKey; }
    KeyCode GetRotateLeftKey() const { return rotateLeftKey; }
    KeyCode GetRotateRightKey() const { return rotateRightKey; }

    // Setters
    void SetMaxForwardSpeed(float speed) { maxForwardSpeed = speed; }
    void SetMaxTurnSpeed(float speed) { maxTurnSpeed = speed; }
    void SetForwardKey(KeyCode key) { forwardKey = key; }
    void SetBackwardKey(KeyCode key) { backwardKey = key; }
    void SetRotateLeftKey(KeyCode key) { rotateLeftKey = key; }
    void SetRotateRightKey(KeyCode key) { rotateRightKey = key; }

private:
    float maxForwardSpeed; // Units per second
    float maxTurnSpeed; // Radians per second

    KeyCode forwardKey;
    KeyCode backwardKey;
    KeyCode rotateLeftKey;
    KeyCode rotateRightKey;
};
