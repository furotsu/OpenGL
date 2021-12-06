#include <iostream>

#include "model.h"

std::vector<std::shared_ptr<Texture>> Model::m_textures = {};

Model::Model(char *path)
{
	loadModel(path);
}

Model::Model(std::string path)
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_directory = path.substr(0, path.find_last_of('/') + 1);
	loadModel(path);
}

void Model::draw(ShaderProgram& shader)
{
	for (unsigned int i = 0; i != m_meshes.size(); ++i)
	{
		m_meshes[i]->setTextureUniform(shader);
		m_meshes[i]->draw(shader);
	}
}

/*
* Restrictions:
*				Scenes - 1;
*				No animations;
*				No bones;
*				Primitives have one set of attributes
*/
void Model::loadModel(std::string path)
{
	sceneStructure::Scene scene(parseglTF(path));

	for (std::shared_ptr<sceneStructure::Node> elem : scene.nodes)
	{
		if (elem->mesh)
			processNode(elem);
	}
}

Json::Value Model::parseglTF(std::string path)
{
	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string rawJson = ss.str();

#ifdef _DEBUG
	if (!file)
	{
		std::cout << "cannot read model file" << std::endl;
	}
#endif
	ss.clear();
	file.close();

	const auto rawJsonLength = static_cast<int>(rawJson.length());

	Json::Value root;
	JSONCPP_STRING err;

	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root, &err))
	{
		std::cout << "cannot parse gltf file" << std::endl;
	}

	return root;
}

void Model::processNode(std::shared_ptr<sceneStructure::Node> node)
{
	m_meshes.push_back(processMesh(*node->mesh, node));
}

std::unique_ptr<Mesh> Model::processMesh(sceneStructure::Mesh &mesh, std::shared_ptr<sceneStructure::Node> node)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	// TODO make checks

	std::ifstream file(m_directory + mesh.primitives[0].position->bufferView->buffer->uri, std::ios::binary | std::ios::in);

	if (!file)
	{
		std::cout << "cannot open bin file for retrieving vertex data" << std::endl;
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	//assume that there is only one primitive
	proccessPositions(positions, mesh.primitives[0], file);
	proccessNormals(normals, mesh.primitives[0], file);
	proccessTexCoords(texCoords, mesh.primitives[0], file);
	processIndices(indices, mesh.primitives[0], file);
	processTextures(textures, mesh.primitives[0]);
	//textures.push_back(Texture(m_directory + mesh.primitives[0].material->baseColorTexture->uri, TextureType::Diffuse));

	file.close();
	for (int i = 0; i != positions.size(); ++i)
	{
		Vertex vertex;
		vertex.Position = positions[i];
		vertex.Normal = normals[i];
		vertex.TexCoords = texCoords[i];
		vertices.push_back(vertex);
	}
	std::unique_ptr<Mesh> m;
	if (node->haveMatrix)
		 m = std::make_unique<Mesh>(vertices, indices, textures, node->transMat);
	else
		 m = std::make_unique<Mesh>(vertices, indices, textures, node->translation, node->rotation, node->scale);
	return std::move(m);
}

// TODO maybe have to template this
void Model::proccessPositions(std::vector<glm::vec3>& positions, sceneStructure::Primitive& primitive, std::ifstream &file)
{
	std::shared_ptr<sceneStructure::Accessor> accessor = primitive.position;
	std::shared_ptr<sceneStructure::BufferView> bufferView = accessor->bufferView;

	unsigned int offset = accessor->byteOffset + bufferView->byteOffset;
	file.seekg(offset, file.beg);
	
	// assume that type == vec3
	unsigned int count = accessor->count;
	//assume that component type == float
	//ComponentType compType = accessor->componentType;

	glm::vec3 position;
	for (int i = 0; i != count; ++i)
	{
		file.read((char*)&position.x, sizeof(float));
		file.read((char*)&position.y, sizeof(float));
		file.read((char*)&position.z, sizeof(float));

		positions.push_back(position);
	}
}

void Model::proccessNormals(std::vector<glm::vec3>& normals, sceneStructure::Primitive& primitive, std::ifstream& file)
{
	std::shared_ptr<sceneStructure::Accessor> accessor = primitive.normal;
	std::shared_ptr<sceneStructure::BufferView> bufferView = accessor->bufferView;

	unsigned int offset = accessor->byteOffset + bufferView->byteOffset;
	file.seekg(offset, file.beg);

	// assume that type == vec3
	unsigned int count = accessor->count;
	//assume that component type == unsigned int
	//ComponentType compType = accessor->componentType;

	glm::vec3 normal;
	for (int i = 0; i != count; ++i)
	{
		file.read((char*)&normal.x, sizeof(float));
		file.read((char*)&normal.y, sizeof(float));
		file.read((char*)&normal.z, sizeof(float));

		normals.push_back(normal);
	}
}

void Model::proccessTexCoords(std::vector<glm::vec2>& texCoords, sceneStructure::Primitive& primitive, std::ifstream& file)
{
	std::shared_ptr<sceneStructure::Accessor> accessor = primitive.texCoord;
	std::shared_ptr<sceneStructure::BufferView> bufferView = accessor->bufferView;

	unsigned int offset = accessor->byteOffset + bufferView->byteOffset;
	file.seekg(offset, file.beg);

	// assume that type == vec2
	unsigned int count = accessor->count;
	//assume that component type == float
	//ComponentType compType = accessor->componentType;

	glm::vec2 texCoord;
	for (int i = 0; i != count; ++i)
	{
		file.read((char*)&texCoord.x, sizeof(float));
		file.read((char*)&texCoord.y, sizeof(float));
 
		texCoords.push_back(texCoord);
	}
}

void Model::processIndices(std::vector<unsigned short>& indices, sceneStructure::Primitive& primitive, std::ifstream& file)
{
	//unsigned short
	std::shared_ptr<sceneStructure::Accessor> accessor = primitive.indices;
	std::shared_ptr<sceneStructure::BufferView> bufferView = accessor->bufferView;

	unsigned int offset = accessor->byteOffset + bufferView->byteOffset;
	file.seekg(offset, file.beg);

	// assume that type == Scalar
	unsigned short count = accessor->count;

	unsigned short n;
	for (int i = 0; i != count; ++i)
	{
		file.read((char*)&n, sizeof(unsigned short));

		indices.push_back(n);
	}
}

void Model::processTextures(std::vector<std::shared_ptr<Texture>>& textures, sceneStructure::Primitive& primitive)
{
	for (int i = 0; i != m_textures.size(); ++i)
	{
		// if texture already loaded send its pointer to mesh
		if (m_textures[i]->filepath == m_directory + primitive.material->baseColorTexture->uri)
		{
			textures.push_back(m_textures[i]);
			return;
		}
	}
	// else - load this texture and send its pointer to mesh
	m_textures.push_back(std::make_shared<Texture>(m_directory + primitive.material->baseColorTexture->uri, TextureType::Diffuse));
	textures.push_back(m_textures[m_textures.size() - 1]);
}

void Model::setPosition(glm::vec3 translation)
{
	for (auto& elem : m_meshes)
		elem->move(translation);
	m_position += translation;
}

void Model::rotate(glm::vec4 rotation)
{
	for (auto& elem : m_meshes)
		elem->rotate(rotation);
}

void Model::scale(glm::vec3 scale)
{
	for (auto& elem : m_meshes)
		elem->scale(scale);
	//m_scale += scale;
}




