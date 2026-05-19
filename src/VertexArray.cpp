#include "VertexArray.h"

// Built-in includes


// Third-party includes
#include <glad/glad.h>

// Local includes
#include "Logging.h"



VertexArray::VertexArray(const Types::Vertex8* vertices, unsigned int vertexCount,
    const Types::Index* indices, unsigned int indexCount)
    : vertexCount(vertexCount), indexCount(indexCount * 3) {
    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Types::Vertex8), vertices, GL_STATIC_DRAW);

    // Generate and bind the element buffer object
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(Types::Index), indices, GL_STATIC_DRAW);

    // Define the vertex attributes
    // Assuming the vertex format is: position (3 floats), normal (3 floats), texCoords (2 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

}

VertexArray::VertexArray(const Types::Vertex3* vertices, unsigned int vertexCount,
    const Types::UV* uvs, unsigned int uvCount,
    const Types::Index* indices, unsigned int indexCount)
    : vertexCount(vertexCount), indexCount(indexCount * 3) {

    //Placeholder normals since they are not provided in the input data; will be calculated in the shader if needed
    Types::Normal placeholderNormal = {0.0f, 0.0f, 1.0f};

    // generate Vertex8 data from the provided Vertex3 and UV data
    std::vector<Types::Vertex8> vertexData;
    vertexData.resize(static_cast<size_t>(vertexCount));
    for (unsigned int i = 0; i < vertexCount; ++i) {
        Types::CreateVertex8(vertices[i], placeholderNormal, uvs[i], vertexData[i]);
    }
    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Types::Vertex8), vertexData.data(), GL_STATIC_DRAW);

    // Generate and bind the element buffer object
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(Types::Index), indices, GL_STATIC_DRAW);

    // Define the vertex attributes
    // Assuming the vertex format is: position (3 floats), normal (3 floats), texCoords (2 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

}

VertexArray::VertexArray(const Types::Vertex3* vertices, unsigned int vertexCount,
    const Types::UV* uvs, unsigned int uvCount,
    const Types::Vertex3* normals, unsigned int normalCount,
    const Types::Index* indices, unsigned int indexCount)
    : vertexCount(vertexCount), indexCount(indexCount * 3) {

    // generate Vertex8 data from the provided Vertex3, UV, and normal data
    std::vector<Types::Vertex8> vertexData;
    vertexData.resize(static_cast<size_t>(vertexCount));
    for (unsigned int i = 0; i < vertexCount; ++i) {
        Types::CreateVertex8(vertices[i], normals[i], uvs[i], vertexData[i]);
    }
    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Types::Vertex8), vertexData.data(), GL_STATIC_DRAW);

    // Generate and bind the element buffer object
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(Types::Index), indices, GL_STATIC_DRAW);

    // Define the vertex attributes
    // Assuming the vertex format is: position (3 floats), normal (3 floats), texCoords (2 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex8), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

}


VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void VertexArray::Bind() const {
    glBindVertexArray(vao);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}
