#include "Mesh.h"

// Built-in includes

// Third-party includes
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>

// Local includes
#include "Config.h"
#include "Logging.h"
#include "Math.h"
#include "Renderer.h"
#include "Texture.h"
#include "Types.h"
#include "VertexArray.h"
#include "Vector.h"

using Vector::Vector3;
using Vector::Vector2;
using Types::Vertex8;
using Types::Index;

Mesh::Mesh()
: vao(nullptr), radius(0.0f), specPower(0.0f) {
    // Constructor code here
}

Mesh::~Mesh() {
    Unload();
}

void Mesh::Load(const std::string& fileName, Renderer* renderer) {
    Unload();

    if (!renderer) {
        Logging::LogError("Mesh::Load called with null renderer for file: %s", fileName.c_str());
        return;
    }

    // Currently only the custom .gp format is supported.
    const std::size_t extPos = fileName.find_last_of('.');
    const bool isGp = (extPos != std::string::npos) && (fileName.substr(extPos) == ".gpmesh");

    if (!isGp) {
        Logging::LogError("Unsupported mesh format for file: %s", fileName.c_str());
        return;
    }

    if (!LoadGPMesh(fileName, renderer)) {
        Logging::LogError("Failed to load mesh: %s", fileName.c_str());
    }
}

void Mesh::Load(PrimitiveShape shape, Renderer* renderer) {
    // Load mesh data for a built-in primitive shape (e.g., cube, sphere, plane)
    // This is a placeholder implementation; actual loading code would depend on the shape
    Unload();

    std::vector<Types::Vertex8> vertices;
    std::vector<Types::Index> indices;

    switch (shape) {
        case PrimitiveShape::Cube:
            // Populate vertices and indices for a cube
            break;
        case PrimitiveShape::Sphere:
            // Populate vertices and indices for a sphere
            break;
        case PrimitiveShape::Plane:
            // Populate vertices and indices for a plane
            break;
        default:
            Logging::LogError("Unsupported primitive shape");
            return;
    }

    Load(vertices, indices, renderer);
}

void Mesh::Load(const std::vector<Types::Vertex8>& vertices, const std::vector<Types::Index>& indices, Renderer* renderer) {
    Unload();

    if (vertices.empty() || indices.empty()) {
        Logging::LogError("Empty vertex or index data provided to Mesh::Load");
        return;
    }

    vao = new VertexArray(vertices.data(), static_cast<unsigned int>(vertices.size()), indices.data(), static_cast<unsigned int>(indices.size()));
}

void Mesh::LoadRawMesh(const std::vector<Types::Vertex3>& vertices, const std::vector<Types::UV>& uvs, const std::vector<Types::Index>& indices, const std::vector<Texture*>& textures, Renderer* renderer) {
    Unload();

    if (vertices.empty() || indices.empty()) {
        Logging::LogError("Empty vertex or index data provided to Mesh::LoadRawMesh");
        return;
    }

    vao = new VertexArray(vertices.data(), static_cast<unsigned int>(vertices.size()), uvs.data(), static_cast<unsigned int>(uvs.size()), indices.data(), static_cast<unsigned int>(indices.size()));
    this->textures = textures;
}

void Mesh::LoadRawMesh(const std::vector<Types::Vertex3>& vertices, const std::vector<Types::UV>& uvs, const std::vector<Types::Vertex3>& normals, const std::vector<Types::Index>& indices, const std::vector<Texture*>& textures, Renderer* renderer) {
    Unload();

    if (vertices.empty() || indices.empty()) {
        Logging::LogError("Empty vertex or index data provided to Mesh::LoadRawMesh with normals");
        return;
    }

    vao = new VertexArray(vertices.data(), static_cast<unsigned int>(vertices.size()), uvs.data(), static_cast<unsigned int>(uvs.size()), normals.data(), static_cast<unsigned int>(normals.size()), indices.data(), static_cast<unsigned int>(indices.size()));
    this->textures = textures;
}

void Mesh::Unload() {
    // Unload mesh data and delete OpenGL buffers
    if (vao) {
        delete vao;
        vao = nullptr;
    }
    // Textures are owned/cached by Renderer::GetTexture.
    textures.clear();
}



bool Mesh::LoadGPMesh(const std::string& fileName, Renderer* renderer) {
    // Load mesh data from a custom .gp format
    // This is a placeholder implementation; actual loading code would depend on the .gp file structure

    std::ifstream file(fileName);
    if (!file.is_open()) {

        Logging::LogError("Failed to open .gp mesh file: %s", fileName.c_str());
        return false;
    }

    // Parse the .gp file and populate vao, textures, shaderName, radius, specPower, etc.

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fileContent = buffer.str();
    rapidjson::StringStream ss(fileContent.c_str());
    rapidjson::Document doc;
    doc.ParseStream(ss);

    if (doc.HasParseError() || !doc.IsObject()) {
        Logging::LogError("Invalid .gp mesh file format: %s", fileName.c_str());
        return false;
    }

    if (!doc.HasMember("version") || !doc["version"].IsInt()) {
        Logging::LogError("Missing or invalid 'version' in file: %s", fileName.c_str());
        return false;
    }
    int version = doc["version"].GetInt();
    if (version != 1) {
        Logging::LogError("Unsupported .gp mesh version: %d in file: %s", version, fileName.c_str());
        return false;
    }

    if (!doc.HasMember("vertexformat") || !doc["vertexformat"].IsString()) {
        Logging::LogError("Missing or invalid 'vertexformat' in file: %s", fileName.c_str());
        return false;
    }
    const std::string vertexFormat = doc["vertexformat"].GetString();
    if (vertexFormat != "PosNormTex") {
        Logging::LogError("Unsupported vertex format '%s' in file: %s", vertexFormat.c_str(), fileName.c_str());
        return false;
    }

    const rapidjson::SizeType vertSize = 8;

    if (!doc.HasMember("textures")) {
        Logging::LogError("Missing 'textures' in file: %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& textureArray = doc["textures"];

    if (!textureArray.IsArray() || textureArray.Size() < 1) {
        Logging::LogError("Invalid .gp mesh file format: 'textures' should be an array in file: %s", fileName.c_str());
        return false;
    }

    if (!doc.HasMember("specularPower") || !doc["specularPower"].IsNumber()) {
        Logging::LogError("Missing or invalid 'specularPower' in file: %s", fileName.c_str());
        return false;
    }
    specPower = static_cast<float>(doc["specularPower"].GetDouble());

    for (rapidjson::SizeType i = 0; i < textureArray.Size(); i++) {
        const rapidjson::Value& texPath = textureArray[i];
        if (!texPath.IsString()) {
            Logging::LogError("Invalid .gp mesh file format: 'textures' array should contain strings in file: %s", fileName.c_str());
            return false;
        }
        Texture* tex = renderer->GetTexture(texPath.GetString());
        if(!tex) {
            tex = renderer->GetTexture(Config::DEFAULT_TEXTURE);
        }

        this->textures.push_back(tex);
    }

    // Parse the .gp file and populate vao, textures, shaderName, radius, specPower, etc.



    if (!doc.HasMember("shader") || !doc["shader"].IsString()) {
        Logging::LogError("Missing or invalid 'shader' in file: %s", fileName.c_str());
        return false;
    }
    shaderName = doc["shader"].GetString();

    if (!doc.HasMember("vertices")) {
        Logging::LogError("Missing 'vertices' in file: %s", fileName.c_str());
        return false;
    }

    const rapidjson::Value& vertsJson = doc["vertices"];
    if (!vertsJson.IsArray()) {
        Logging::LogError("Invalid .gp mesh file format: 'vertices' should be an array in file: %s", fileName.c_str());
        return false;
    }

    radius = 0.0f;

    std::vector<Vertex8> vertexData;
    vertexData.resize(static_cast<size_t>(vertsJson.Size()));
    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
        const rapidjson::Value& vert = vertsJson[i];
        if (!vert.IsArray() || vert.Size() != vertSize) {
            Logging::LogError("Invalid .gp mesh file format: each vertex should be an array of size %zu in file: %s", vertSize, fileName.c_str());
            return false;
        }

        for (rapidjson::SizeType j = 0; j < vertSize; ++j) {
            if (!vert[j].IsNumber()) {
                Logging::LogError("Invalid vertex component type at vertex %u, component %u in file: %s",
                    static_cast<unsigned>(i), static_cast<unsigned>(j), fileName.c_str());
                return false;
            }
        }

        Vector3 position(vert[0].GetFloat(), vert[1].GetFloat(), vert[2].GetFloat());
        radius = Math::Max(radius, Vector3::Length(position));

        Vector3 normal(vert[3].GetFloat(), vert[4].GetFloat(), vert[5].GetFloat());
        Vector2 texCoords(vert[6].GetFloat(), vert[7].GetFloat());
        Types::CreateVertex8(position, normal, texCoords, vertexData[i]);
    }

    if (!doc.HasMember("indices")) {
        Logging::LogError("Missing 'indices' in file: %s", fileName.c_str());
        return false;
    }
    const rapidjson::Value& indicesJson = doc["indices"];
    if (!indicesJson.IsArray()) {
        Logging::LogError("Invalid .gp mesh file format: 'indices' should be an array in file: %s", fileName.c_str());
        return false;
    }

    std::vector<Index> indexData;
    indexData.resize(static_cast<size_t>(indicesJson.Size()));

    for (rapidjson::SizeType i = 0; i < indicesJson.Size(); i++) {
        const rapidjson::Value& index = indicesJson[i];
        if (!index.IsArray() || index.Size() != 3) {
            Logging::LogError("Invalid .gp mesh file format: each index should be an array of size 3 in file: %s", fileName.c_str());
            return false;
        }

        for (rapidjson::SizeType j = 0; j < 3; ++j) {
            if (!index[j].IsUint()) {
                Logging::LogError("Invalid index component type at triangle %u, component %u in file: %s",
                    static_cast<unsigned>(i), static_cast<unsigned>(j), fileName.c_str());
                return false;
            }
            const unsigned int value = index[j].GetUint();
            if (value >= static_cast<unsigned int>(vertsJson.Size())) {
                Logging::LogError("Index out of range at triangle %u, component %u (value %u, max %u) in file: %s",
                    static_cast<unsigned>(i), static_cast<unsigned>(j), value,
                    static_cast<unsigned>(vertsJson.Size() - 1), fileName.c_str());
                return false;
            }
            indexData[static_cast<size_t>(i)][static_cast<size_t>(j)] = value;
        }
    }

    vao = new VertexArray(
        vertexData.data(),
        static_cast<unsigned>(vertsJson.Size()),
        indexData.data(),
        static_cast<unsigned>(indicesJson.Size()));

    return true;
}