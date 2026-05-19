#include "Actor.h"

// Built-in includes
#include <algorithm>

// Third-party includes

// Local includes
#include "Game.h"
#include "Component.h"

Actor::Actor(class Game* game)
: state(Active),
  rotation(0.0f),
  scale(Vector::Vector3::One * 2.0f),
  position(Vector::Vector3::Zero),
  worldTransform(Matrix::Matrix4::Identity),
  recomputeWorldTransform(true),
  game(game)
{
    game->AddActor(this);
}

Actor::~Actor() {
    // Remove all components
    game->RemoveActor(this);
    while (!components.empty()) {
        delete components.back();
        components.pop_back();
    }
}

void Actor::Update(float deltaTime) {
    if (state == Active) {
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
        if (recomputeWorldTransform) {
            ComputeWorldTransform();
        }
    }
}

void Actor::UpdateComponents(float deltaTime) {
    for (auto component : components) {
        component->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime) {
    // Default implementation (can be overridden by derived classes)
}

void Actor::ProcessInput() {
    if (state == Active) {
        for (auto component : components) {
            component->ProcessInput();
        }
        ActorInput(SDL_GetKeyboardState(NULL));
    }
}

void Actor::ActorInput(const bool* state) {
    // Default implementation (can be overridden by derived classes)
}

void Actor::AddComponent(Component* component) {
    auto iter = components.begin();
    while (iter != components.end()) {
        if ((*iter)->GetUpdateOrder() > component->GetUpdateOrder()) {
            break;
        }
        ++iter;
    }
    components.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
    auto iter = std::find(components.begin(), components.end(), component);
    if (iter != components.end()) {
        components.erase(iter);
    }
}

void Actor::ComputeWorldTransform() {
    if (!recomputeWorldTransform) {
        return;
    }
    recomputeWorldTransform = false;

    Matrix::Matrix4 scaleMat = Matrix::Matrix4::Scaling(scale);
    Matrix::Matrix4 rotMat = Matrix::Matrix4::RotationZ(rotation.z)
        * Matrix::Matrix4::RotationY(rotation.y)
        * Matrix::Matrix4::RotationX(rotation.x);
    Matrix::Matrix4 transMat = Matrix::Matrix4::Translation(position);

    // Column-vector order: scale, then rotate, then translate.
    worldTransform = transMat * rotMat * scaleMat;

    for (auto component : components) {
        component->OnComputeWorldTransform();
    }
}