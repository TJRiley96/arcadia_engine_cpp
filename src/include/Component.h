#pragma once

// Built-in includes

// Third-party includes

// Local includes


class Component {
public:
    Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime);
    virtual void ProcessInput();
    virtual void OnComputeWorldTransform();

    int GetUpdateOrder() const { return updateOrder; }

protected:
    class Actor* owner; // Forward declaration of Actor class

    int updateOrder; // Used to determine update order of components
};