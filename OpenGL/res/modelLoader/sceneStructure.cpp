#include <iostream>	

#include "sceneStructure.h"

sceneStructure::Scene::Scene(const Json::Value& root)
{

	for (auto& elem : root["buffers"])
		this->buffers.push_back(std::make_shared<Buffer>(elem));
	
	for (auto& elem : root["bufferViews"])
		this->bufferViews.push_back(std::make_shared<BufferView>(elem, *this));

	for (auto& elem : root["images"])
		this->images.push_back(elem["uri"].asString());

	for (auto& elem : root["textures"])
		this->textures.push_back(std::make_shared<Texture>(elem, *this));

	for (auto& elem : root["materials"])
		this->materials.push_back(std::make_shared<Material>(elem, *this));

	for (auto& elem : root["accessors"])
		this->accessors.push_back(std::make_shared<Accessor>(elem, *this));

	for (auto& elem : root["meshes"])
		this->meshes.push_back(std::make_shared<Mesh>(elem, *this));

	for (auto &elem : root["nodes"])
		this->nodes.push_back(std::make_shared<Node>(elem, *this));

	for (auto &elem : this->nodes)
	{
		for (int i = 0; i < elem->childrenId.size(); i++)
		{
			elem->children.push_back(this->nodes[elem->childrenId[i]]);
			this->nodes[elem->childrenId[i]]->setParentParams(elem);
		}
	}
}

sceneStructure::Node::Node(const Json::Value& node, Scene& scene)
{
	this->mesh = node.isMember("mesh") ? scene.meshes[node["mesh"].asInt()] : nullptr;
	this->name = node.isMember("name") ? node["name"].asString() : "unnamed";

	if (node.isMember("children"))
	{
		for (auto& elem : node["children"])
		{
			this->childrenId.push_back(elem.asInt());
		}
	}

	if (node.isMember("translation"))
	{
		this->translation.x = node["translation"][0].asFloat();
		this->translation.y = node["translation"][1].asFloat();
		this->translation.z = node["translation"][2].asFloat();
	}
	else
		this->translation = glm::vec3(0.0f);

	if (node.isMember("rotation"))
	{
		this->rotation[0] = node["rotation"][0].asFloat();
		this->rotation[1] = node["rotation"][1].asFloat();
		this->rotation[2] = node["rotation"][2].asFloat();
		this->rotation[3] = node["rotation"][3].asFloat();
	}
	else
		this->rotation = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (node.isMember("scale"))
	{
		this->scale.x = node["scale"][0].asFloat();
		this->scale.y = node["scale"][1].asFloat();
		this->scale.z = node["scale"][2].asFloat();
	}
	else
		this->scale = glm::vec3(1.0f);

	if (node.isMember("matrix"))
	{
		for (int i = 0; i != 4; ++i)
		{

			this->transMat[i][0] = node["matrix"][i * 4].asFloat();
			this->transMat[i][1] = node["matrix"][i * 4 + 1].asFloat();
			this->transMat[i][2] = node["matrix"][i * 4 + 2].asFloat();
			this->transMat[i][3] = node["matrix"][i * 4 + 3].asFloat();
		}
		haveMatrix = true;
	}
	else
		haveMatrix = false;
}

void sceneStructure::Node::setParentParams(const std::shared_ptr<Node> parentNode) 
{
	if (parentNode->haveMatrix)
	{
		this->haveMatrix = true;
		this->transMat = parentNode->transMat;
	}
	else
	{
		this->translation = (parentNode->translation != glm::vec3(0.0f)) ? parentNode->translation : this->translation;
		this->rotation = (parentNode->rotation != glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) ? parentNode->rotation : this->rotation;
		this->scale = (parentNode->scale != glm::vec3(1.0f)) ? parentNode->scale : this->scale;
	}
}

sceneStructure::Mesh::Mesh(const Json::Value& mesh, Scene& scene)
{
	this->name = mesh.isMember("name") ? mesh["name"].asString() : "unnamed";
	for (auto& elem : mesh["primitives"])
	{
		this->primitives.push_back(Primitive(elem, scene));
	}
}

sceneStructure::Primitive::Primitive(const Json::Value& primitive, Scene& scene)
{
	auto& attributes = primitive["attributes"];

	this->position = attributes.isMember("POSITION") ? scene.accessors[attributes["POSITION"].asInt()] : nullptr;
	this->normal = attributes.isMember("NORMAL") ? scene.accessors[attributes["NORMAL"].asInt()] : nullptr;
	this->texCoord = attributes.isMember("TEXCOORD_0") ? scene.accessors[attributes["TEXCOORD_0"].asInt()] : nullptr;
	this->tangent = attributes.isMember("TANGENT") ? scene.accessors[attributes["TANGENT"].asInt()] : nullptr;

	this->indices = primitive.isMember("indices") ? scene.accessors[primitive["indices"].asInt()] : nullptr;
	this->material = primitive.isMember("material") ? scene.materials[primitive["material"].asInt()] : nullptr;
}

sceneStructure::Material::Material(const Json::Value& material, Scene& scene)
{
	bool haveMetRough = material.isMember("pbrMetallicRoughness");
	if (haveMetRough)
	{
		this->baseColorTexture = scene.textures[material["pbrMetallicRoughness"]["baseColorTexture"]["index"].asInt()];
	}
}

//TODO if source is undefined should supply alternate
// Image should be defined only with uri
sceneStructure::Texture::Texture(const Json::Value& texture, Scene& scene)
{
	this->uri = scene.images[texture["source"].asInt()];
}

sceneStructure::Buffer::Buffer(const Json::Value& buffer)
{
	if (!buffer.isMember("uri"))
	{
		std::cout << "Cannot get uri of some buffer" << std::endl;
	}
	this->uri = buffer["uri"].asString();
	this->byteLength = buffer["byteLenght"].asUInt();
}

sceneStructure::BufferView::BufferView(const Json::Value& bufferView, Scene& scene)
{
	this->buffer = scene.buffers[bufferView["buffer"].asInt()];
	this->byteLength = bufferView["byteLength"].asUInt();
	this->byteOffset = bufferView.isMember("byteOffset") ? bufferView["byteOffset"].asUInt() : 0;
	this->target = bufferView.isMember("target") ? bufferView["target"].asInt() : -1;
}

sceneStructure::Accessor::Accessor(const Json::Value& accessor, Scene& scene)
{
	if (!accessor.isMember("bufferView"))
	{
		std::cout << "Cannot get bufferView of some buffer" << std::endl;
	}

	this->bufferView = scene.bufferViews[accessor["bufferView"].asInt()];
	this->type = accessor["type"].asString();
	this->count = accessor["count"].asUInt();
	this->componentType = (ComponentType)accessor["componentType"].asInt();
	this->byteOffset = accessor.isMember("byteOffset") ? accessor["byteOffset"].asUInt() : 0;
}

