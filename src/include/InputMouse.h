#pragma once


// Built-in includes

// Third-party includes

// Local includes


class InputMouse {
public:
    InputMouse();
    ~InputMouse();

    void Update();

    // Getters
    float GetX() const { return x; }
    float GetY() const { return y; }
    bool IsLeftButtonDown() const { return leftButtonDown; }
    bool IsRightButtonDown() const { return rightButtonDown; }

private:
    float x;
    float y;
    bool leftButtonDown;
    bool rightButtonDown;
};