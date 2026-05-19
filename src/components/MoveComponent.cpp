#include "MoveComponent.h"

// Built-in includes

// Third-party includes

// Local includes
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
: Component(owner, updateOrder),
  moveSpeed(Vector3::Zero),
  angularSpeed(0.0f),
  pitchSpeed(0.0f),
  elevationSpeed(0.0f) {}

MoveComponent::~MoveComponent() {}

void MoveComponent::  Update(float deltaTime) {
    if (!Math::NearlyZero(moveSpeed.x) || !Math::NearlyZero(moveSpeed.y) || !Math::NearlyZero(moveSpeed.z)) {
        Vector::Vector3 forward = owner->GetForward();
        Vector::Vector3 right = Vector::Vector3::Cross(forward, Vector::Vector3::Up).Normalize();
        Vector::Vector3 moveDir = (right * moveSpeed.x) + (forward * moveSpeed.y) + (Vector::Vector3::Up * moveSpeed.z);
        owner->SetPosition(owner->GetPosition() + moveDir * deltaTime);
    }
    if (!Math::NearlyZero(angularSpeed)) {
        Vector::Vector3 rot = owner->GetRotation();
        rot.y += angularSpeed * deltaTime;
        owner->SetRotation(rot);
    }
    if (!Math::NearlyZero(elevationSpeed)) {
        owner->SetPosition(owner->GetPosition() + Vector::Vector3::Up * elevationSpeed * deltaTime);
    }
    if (!Math::NearlyZero(pitchSpeed)) {
        Vector::Vector3 rot = owner->GetRotation();
        rot.x += pitchSpeed * deltaTime;
        const float maxPitch = Math::HALF_PI - 0.01f;
        rot.x = Math::Clamp(rot.x, -maxPitch, maxPitch);
        owner->SetRotation(rot);
    }
}
