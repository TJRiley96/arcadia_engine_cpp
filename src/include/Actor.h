#pragma once


// Built-in includes
#include <vector>

// Third-party includes

// Local includes
#include "Vector.h"
#include "Matrix.h"

class Actor {
public:

    enum State {
        Active,
        Paused,
        Dead
    };

    Actor(class Game* game);
    ~Actor();

    // Update functions
    void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);

    // Input processing
    void ProcessInput();
    virtual void ActorInput(const bool* state);

    // Getters
    const Vector::Vector3& GetPosition() const { return position; }
    const Vector::Vector3& GetRotation() const { return rotation; }
    const Vector::Vector3& GetScale() const { return scale; }
    State GetState() const { return state; }
    class Game* GetGame() { return game; }


    // Setters
    void SetPosition(const Vector::Vector3& pos) { position = pos; recomputeWorldTransform = true; }
    void SetRotation(const Vector::Vector3& rot) { rotation = rot; recomputeWorldTransform = true; }
    void SetScale(const Vector::Vector3& s) { scale = s; recomputeWorldTransform = true; }
    void SetScale(float s) { scale = Vector::Vector3(s, s, s); recomputeWorldTransform = true; }
    void SetState(State s) { state = s; }

    // World transform
    void ComputeWorldTransform();
    const Matrix::Matrix4& GetWorldTransform() const { return worldTransform; }
    Vector::Vector3 GetForward() const {
        float cosPitch = cosf(rotation.x);
        return Vector::Vector3(-sinf(rotation.y) * cosPitch, cosf(rotation.y) * cosPitch, sinf(rotation.x));
    }

    // Component management
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:

    State state;

    Vector::Vector3 rotation;
    Vector::Vector3 scale;
    Vector::Vector3 position;
    Matrix::Matrix4 worldTransform;

    bool recomputeWorldTransform;

    std::vector<class Component*> components;
    class Game* game;
};