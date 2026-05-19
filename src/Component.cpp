#include "Component.h"

// Built-in includes

// Third-party includes

// Local includes
#include "Actor.h"

Component::Component(class Actor* owner, int updateOrder)
: owner(owner), updateOrder(updateOrder) {
    owner->AddComponent(this);
}

Component::~Component() {
    owner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {
    // Default implementation (can be overridden by derived classes)
}

void Component::ProcessInput() {
    // Default implementation (can be overridden by derived classes)
}

void Component::OnComputeWorldTransform() {
    // Default implementation (can be overridden by derived classes)
}

