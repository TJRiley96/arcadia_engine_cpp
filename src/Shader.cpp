#include "Shader.h"

// Built-in includes
#include <fstream>
#include <sstream>

// Third-party includes
#include <SDL3/SDL.h>

// Local includes
#include "Logging.h"


Shader::Shader()
: shaderProgram(0), vertShaderId(0), fragShaderId(0) {}

Shader::~Shader() {
    Unload();
}

bool Shader::Load(const char* vertexPath, const char* fragmentPath) {
    // Load and compile vertex and fragment shaders, then link them into a shader program
    // Return true if successful, false otherwise

    if(!CompileShader(vertexPath, GL_VERTEX_SHADER, vertShaderId) ||
    !CompileShader(fragmentPath, GL_FRAGMENT_SHADER, fragShaderId)) {
        return false;
    }
    Logging::LogInfo("Shaders compiled successfully");

    // Link shaders into a program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShaderId);
    glAttachShader(shaderProgram, fragShaderId);
    glLinkProgram(shaderProgram);

    if (!IsLinked()) {
        return false;
    }


    Logging::LogInfo("Shaders linked successfully");
    return true;
}

void Shader::Unload() {
    // Clean up shader resources
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    if (vertShaderId != 0) {
        glDeleteShader(vertShaderId);
        vertShaderId = 0;
    }
    if (fragShaderId != 0) {
        glDeleteShader(fragShaderId);
        fragShaderId = 0;
    }
}

void Shader::Use() const {
    // Activate the shader program for rendering
    if (shaderProgram != 0) {
        glUseProgram(shaderProgram);
    }
}

bool Shader::CompileShader(const char* source, GLenum shaderType, GLuint& shaderID) {
    // Compile the shader source code and check for errors

    std::ifstream shaderFile(source);
    if (!shaderFile.is_open()) {
        Logging::LogError("Failed to open shader file");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader File Error", "Failed to open shader file", nullptr);
        return false;
    }
    Logging::LogInfo("Shader file loaded successfully");

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);

    if (!IsCompiled(shaderID)) {
        return false;
    }


    return true;
}

bool Shader::IsCompiled(GLuint shader) {
    // Check if the shader compiled successfully

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        Logging::LogError(infoLog);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Compilation Error", infoLog, nullptr);
        // Log the error message using your logging system
        return false;
    }
    Logging::LogInfo("Shader compiled successfully");
    return true;
}

bool Shader::IsLinked() {
    // Check if the shader program linked successfully

    GLint status;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        Logging::LogError(infoLog);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Linking Error", infoLog, nullptr);
        // Log the error message using your logging system
        return false;
    }

    return true; // Placeholder return value
}

// OpenGL uniform setting functions
void Shader::SetUniform1i(const char* name, int value) const {
    // Set an integer uniform variable in the shader program
    GLint location = glGetUniformLocation(shaderProgram, name);
    if (location != -1) {
        glUniform1i(location, value);
    } else {
        std::string msg = "Uniform integer variable not found: " + std::string(name);
        Logging::LogWarning(msg.c_str());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Shader Uniform Warning", msg.c_str(), nullptr);

    }
}

void Shader::SetUniform1f(const char* name, float value) const {
    // Set a float uniform variable in the shader program
    GLint location = glGetUniformLocation(shaderProgram, name);
    if (location != -1) {
        glUniform1f(location, value);
    } else {
        std::string msg = "Uniform float variable not found: " + std::string(name);
        Logging::LogWarning(msg.c_str());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Shader Uniform Warning", msg.c_str(), nullptr);
    }
}

void Shader::SetUniformMatrix4fv(const char* name, const Matrix4& matrix) const {
    // Set a 4x4 matrix uniform variable in the shader program
    GLint location = glGetUniformLocation(shaderProgram, name);
    if (location != -1) {
        // CPU matrices are built in row-major/row-vector form; transpose on upload
        // so GLSL can use the standard column-vector multiplication order.
        glUniformMatrix4fv(location, 1, GL_TRUE, matrix.GetAsFloatPtr());
    } else {
        std::string msg = "Uniform Matrix variable not found: " + std::string(name);
        Logging::LogWarning(msg.c_str());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Shader Uniform Warning", msg.c_str(), nullptr);
    }
}