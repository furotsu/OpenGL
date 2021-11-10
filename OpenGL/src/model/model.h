#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

//#include "json/value.h"
#include "model/json.h"

#include "sceneStructure.h"

#include "Mesh.h"
#include "Shader.h"

class Model
{
public: //private:
    // model data
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::string m_directory;
public:
    Model(char* path);
    Model(std::string path);

    void draw(ShaderProgram& shader);

public: //private:
    void loadModel(std::string path);
    Json::Value parseglTF(std::string path);
    void processNode(std::shared_ptr<sceneStructure::Node> node);
    std::unique_ptr<Mesh> processMesh(sceneStructure::Mesh &mesh, std::shared_ptr<sceneStructure::Node> node);
    //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
    
    void proccessPositions(std::vector<glm::vec3>& positions, sceneStructure::Primitive& primitive, std::ifstream &file);
    void proccessNormals(std::vector<glm::vec3>& normals, sceneStructure::Primitive& primitive, std::ifstream& file);
    void proccessTexCoords(std::vector<glm::vec2>& texCoords, sceneStructure::Primitive& primitive, std::ifstream& file);
    void processIndices(std::vector<unsigned short>& indices, sceneStructure::Primitive& primitive, std::ifstream& file);
    void processTextures(std::vector<Texture>& textures, sceneStructure::Primitive& primitive);
};  
