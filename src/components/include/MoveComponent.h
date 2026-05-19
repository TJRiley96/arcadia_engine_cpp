#pragma once

// Built-in includes

// Third-party includes

// Local includes
#include "Component.h"
#include "Vector.h"

using Vector::Vector3;

class MoveComponent : public Component {
public:
    MoveComponent(class Actor* owner, int updateOrder = 100);
    ~MoveComponent();

    void Update(float deltaTime) override;

    // Getters
    Vector3 GetMoveSpeed() const { return moveSpeed; }
    float GetAngularSpeed() const { return angularSpeed; }
    float GetPitchSpeed() const { return pitchSpeed; }
    float GetElevationSpeed() const { return elevationSpeed; }

    // Setters
    void SetMoveSpeed(const Vector3& speed) { moveSpeed = speed; }
    void SetAngularSpeed(float speed) { angularSpeed = speed; }
    void SetPitchSpeed(float speed) { pitchSpeed = speed; }
    void SetElevationSpeed(float speed) { elevationSpeed = speed; }

private:
    Vector3 moveSpeed; // Units per second
    float angularSpeed; // Radians per second
    float pitchSpeed; // Radians per second (for future use)
    float elevationSpeed; // Units per second (for future use)
};