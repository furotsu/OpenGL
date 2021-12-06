#pragma once

#include <vector>
#include <string>
#include <utility>

#include <actor/json.h>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

enum class ComponentType
{
	BYTE = 5201,
	UNSIGNED_BYTE = 5121,
	SHORT = 5122,
	UNSIGNED_SHORT = 5123,
	UNSIGNED_INT = 5125,
	FLOAT = 5126
};

namespace sceneStructure
{
	struct Scene;

	struct Buffer
	{
		std::string uri;
		unsigned int byteLength;
		int id;
		
		Buffer(const Json::Value&);
	};

	struct BufferView
	{
		std::shared_ptr<Buffer> buffer;
		unsigned int byteOffset;
		unsigned int byteLength;
		unsigned int target; // ARRAY_BUFFER of ELEMENT_ARRAY_BUFFER
		int id;

		//unsigned int byteStride; //stride between two elements

		BufferView(const Json::Value&, Scene&);
	};

	// TODO add min and max values and sparce accessor
	struct Accessor
	{
		std::shared_ptr<BufferView> bufferView;
		std::string type; // VEC2, VEC3, VEC4, SCALAR
		unsigned int byteOffset;
		unsigned int count;
		ComponentType componentType; 
		int id;

		Accessor(const Json::Value& accessor, Scene& scene);
	};

	struct Sampler
	{

	};

	struct Texture
	{
		std::string uri;
		std::shared_ptr<Sampler> sampler;

		Texture(const Json::Value& texture, Scene& scene);
	};

	struct Material
	{
		std::shared_ptr<Texture> baseColorTexture;

		Material(const Json::Value& material, Scene& scene);
	};

	struct Primitive
	{
		std::shared_ptr<Accessor> position;
		std::shared_ptr<Accessor> normal;
		std::shared_ptr<Accessor> texCoord;
		std::shared_ptr<Accessor> tangent;

		std::shared_ptr<Accessor> indices;
		std::shared_ptr<Material> material;

		Primitive(const Json::Value& primitive, Scene& scene);
	};

	struct Mesh
	{
		std::vector<Primitive> primitives;
		std::string name;
		int id;

		Mesh(const Json::Value& mesh, Scene& scene);
	};

	/*
	*	Contains information about meshes for each part of model.
	*	Each node can contain vector of its child nodes which inherit
	*		its translation, rotation and scale vectors.
	*/
	struct Node
	{
		std::vector<std::shared_ptr<Node>> children;
		std::vector<int> childrenId;
		std::shared_ptr<Mesh> mesh;
		glm::vec3 translation;
		glm::vec4 rotation;
		glm::vec3 scale;
		glm::mat4 transMat;
		std::string name;
		bool haveMatrix;
		int id;

		Node(const Json::Value& node, Scene& scene);

		void setParentParams(const std::shared_ptr<Node>);
	};

	/*
	  The root of each model.
		Contains all the Nodes
	*/
	struct Scene
	{
		std::string name;
		std::vector<std::shared_ptr<Node>> nodes;
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::vector<std::shared_ptr<Accessor>> accessors;
		std::vector<std::shared_ptr<Material>> materials;
		std::vector<std::shared_ptr<Texture>> textures;
		std::vector<std::shared_ptr<BufferView>> bufferViews;
		std::vector<std::shared_ptr<Buffer>> buffers;
		std::vector<std::string> images;

		Scene(const Json::Value& root);
	};
}