#pragma once

// Built-in includes
#include <cmath>
#include <stdint.h>
#include <vector>

// Third-party includes

// Local includes

#include "Types.h"
#include "Vector.h"

using Vector::Vector3;
using Vector::Vector2;


class VertexArray {
public:
    // indexCount constructor argument is triangle count (Types::Index entries).
    // Stored indexCount is element count for glDrawElements.
    VertexArray(const Types::Vertex8* vertices, unsigned int vertexCount,
        const Types::Index* indices, unsigned int indexCount);
    VertexArray(const Types::Vertex3* vertices, unsigned int vertexCount,
        const Types::UV* uvs, unsigned int uvCount,
        const Types::Index* indices, unsigned int indexCount);
    VertexArray(const Types::Vertex3* vertices, unsigned int vertexCount,
        const Types::UV* uvs, unsigned int uvCount,
        const Types::Vertex3* normals, unsigned int normalCount,
        const Types::Index* indices, unsigned int indexCount);
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    // Getters for the vertex array properties
    unsigned int GetID() const { return vao; }
    unsigned int GetVertexCount() const { return vertexCount; }
    unsigned int GetIndexCount() const { return indexCount; }

private:
// Private member variables and functions here

    // OpenGL ID for the vertex array object
    unsigned int vao;
    // OpenGL ID for the vertex buffer object
    unsigned int vbo;
    // OpenGL ID for the element buffer object
    unsigned int ebo;
    // Number of vertices in the vertex array
    unsigned int vertexCount;
    // Number of indices in the element buffer
    unsigned int indexCount;
};