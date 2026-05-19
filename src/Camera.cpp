#include "Camera.h"

// Built-in includes

// Third-party includes
#include <SDL3/SDL.h>

// Local includes
#include "Game.h"
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Vector.h"
#include "MoveComponent.h"

using Matrix::Matrix4;
using Vector::Vector3;


Camera::Camera(class Game* game)
    : Actor(game) {
    // Set initial position and rotation
	SetPosition(Vector3(0.0f, -900.0f, 420.0f));
    SetRotation(0.0f);
    // Set initial speeds
    mc = new MoveComponent(this);

    // angularSpeed = Math::PI; // Radians per second
    // forwardSpeed = 5.0f; // Units per second
}

void Camera::UpdateActor(float deltaTime) {
    // Update camera position and rotation based on input
    // Vector3 forward = GetForward();
    // SetPosition(GetPosition() + forward * forwardSpeed * deltaTime);
    // SetRotation(GetRotation() + angularSpeed * deltaTime);
    Actor::UpdateActor(deltaTime);

	// Compute new camera from this actor
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 120.0f - Vector3::Up * 60.0f;
	Vector3 up = Vector3::Up;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void Camera::ActorInput(const bool* keys) {
    Vector3 moveSpeed = Vector3::Zero;
	float angularSpeed = 0.0f;
	float pitchSpeed = 0.0f;
    float elevationSpeed = 0.0f;

	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		moveSpeed += Vector3::Forward * 300.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		moveSpeed -= Vector3::Forward * 300.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		moveSpeed -= Vector3::Right * 300.0f;
	}
	if (keys[SDL_SCANCODE_D])
	{
		moveSpeed += Vector3::Right * 300.0f;
	}

	// rotation
	if (keys[SDL_SCANCODE_LEFT])
	{
		angularSpeed += Math::TWO_PI;
	}
	if (keys[SDL_SCANCODE_RIGHT])
	{
		angularSpeed -= Math::TWO_PI;
	}

	if (keys[SDL_SCANCODE_UP]) {
		pitchSpeed += Math::TWO_PI;
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		pitchSpeed -= Math::TWO_PI;
	}

	// elevation
    if (keys[SDL_SCANCODE_SPACE]) {
        elevationSpeed += 300.0f;
    }
    if (keys[SDL_SCANCODE_LSHIFT]) {
        elevationSpeed -= 300.0f;
    }

	mc->SetMoveSpeed(moveSpeed);
	mc->SetAngularSpeed(angularSpeed);
    mc->SetPitchSpeed(pitchSpeed);
    mc->SetElevationSpeed(elevationSpeed);
}
