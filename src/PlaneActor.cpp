#include "PlaneActor.h"

// Built-in includes

// Third-party includes

// Local includes
#include "Renderer.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"

PlaneActor::PlaneActor(class Game* game)
    : Actor(game) {
    // Create mesh component
    SetScale(Vector3(100.0f, 1.0f, 100.0f));
    MeshComponent* meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(game->GetRenderer()->GetMesh("assets/obj/Plane.gpmesh"));
}