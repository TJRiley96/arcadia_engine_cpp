#pragma once

// Built-in includes

// Third-party includes
#include <glad/glad.h>

// Local includes
#include "Matrix.h"

using Matrix::Matrix4;

class Shader {
public:
    Shader();
    ~Shader();

    bool Load(const char* vertexPath, const char* fragmentPath);
    void Unload();

    void Use() const;

    void SetUniform1i(const char* name, int value) const;
    void SetUniform1f(const char* name, float value) const;
    void SetUniformMatrix4fv(const char* name, const Matrix4& matrix) const;

private:
// Private member variables and functions here
    // OpenGL ID for the shader program
    GLuint shaderProgram;
    GLuint fragShaderId;
    GLuint vertShaderId;

    bool CompileShader(const char* source, GLenum shaderType, GLuint& shaderID);

    // Helper functions to check for shader compilation and program linking errors
    bool IsCompiled(GLuint shader);
    bool IsLinked();

};