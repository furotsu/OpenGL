#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

//#include "json/value.h"
#include "actor/json.h"

#include "sceneStructure.h"

#include "Mesh.h"
#include "Shader.h"

class Model
{
public: //private:
    // model data
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    static std::vector<std::shared_ptr<Texture>> m_textures;
    std::string m_directory;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec4 m_rotation;
public:
    Model(char* path);
    Model(std::string path);

    void draw(ShaderProgram& shader);

public: //private:
    void loadModel(std::string path);
    Json::Value parseglTF(std::string path);
    void processNode(std::shared_ptr<sceneStructure::Node> node);
    std::unique_ptr<Mesh> processMesh(sceneStructure::Mesh &mesh, std::shared_ptr<sceneStructure::Node> node);
    
    void proccessPositions(std::vector<glm::vec3>& positions, sceneStructure::Primitive& primitive, std::ifstream &file);
    void proccessNormals(std::vector<glm::vec3>& normals, sceneStructure::Primitive& primitive, std::ifstream& file);
    void proccessTexCoords(std::vector<glm::vec2>& texCoords, sceneStructure::Primitive& primitive, std::ifstream& file);
    void processIndices(std::vector<unsigned short>& indices, sceneStructure::Primitive& primitive, std::ifstream& file);
    void processTextures(std::vector<std::shared_ptr<Texture>>& textures, sceneStructure::Primitive& primitive);

public:
    void setPosition(glm::vec3 translation);
    void rotate(glm::vec4 rotation);
    void scale(glm::vec3 scale);

    friend class Gui;
};  
