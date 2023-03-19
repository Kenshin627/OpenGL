#include <iostream>
#include "sceneLoader.h"

SceneLoader::SceneLoader() {}
SceneLoader::~SceneLoader() {}

std::shared_ptr<Node> SceneLoader::loadModel(const std::string& path)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR:ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}
	directory = path.substr(0, path.find_last_of('/'));
	return processNode(scene->mRootNode, scene, nullptr);
}

std::shared_ptr<Node> SceneLoader::processNode(aiNode* node, const aiScene* scene, std::shared_ptr<Node> parent)
{
	std::shared_ptr<Node> current = std::make_shared<Node>();
	current->parent = parent;
	current->nodeName = node->mName.C_Str();
	if (!node->mTransformation.IsIdentity())
	{
		current->modelMatrix = SceneLoader::transformaiMatrix4x4(node->mTransformation);
	}
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		current->meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		current->children.push_back(processNode(node->mChildren[i], scene, current));
	}
	return current;
}

std::shared_ptr<Mesh> SceneLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
	Vertex vertex;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{	
		if (mesh->HasPositions())
		{
			auto p = mesh->mVertices[i];
			vertex.position = glm::vec3(p.x, p.y, p.z);
		}
		//永远返回true,设置了postprocess: aiProcess_GenSmoothNormals
		if (mesh->HasNormals())
		{
			auto n = mesh->mNormals[i];
			vertex.normal = glm::vec3(n.x, n.y, n.z);
		}
		unsigned uvChannels = mesh->GetNumUVChannels();
		for (size_t j = 0; j < uvChannels; j++)
		{
			auto uvs = mesh->mTextureCoords[j][i];
			vertex.uvs.emplace_back(uvs.x, uvs.y, uvs.z);
		}
		vertices.push_back(vertex);
		vertex.uvs.clear();
	}

	if (mesh->HasFaces())
	{
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			auto face = mesh->mFaces[i];
			//三角化 face.mNumIndices返回3
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}

	unsigned materialIndex = mesh->mMaterialIndex;
	std::shared_ptr<Material> mat = nullptr;
	if (materialIndex >= 0)
	{
		auto cachedMaterial = materialCache.find(materialIndex);
		if (cachedMaterial != materialCache.cend())
		{
			mat = cachedMaterial->second;
		}
		else
		{
			mat = std::make_shared<Material>();
			aiColor3D colorTemp;
			float shininess;
			aiMaterial* material = scene->mMaterials[materialIndex];
			material->Get(AI_MATKEY_COLOR_AMBIENT, colorTemp);
			mat->ambient = glm::vec3(colorTemp.r, colorTemp.g, colorTemp.b);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, colorTemp);
			mat->diffuse = glm::vec3(colorTemp.r, colorTemp.g, colorTemp.b);
			material->Get(AI_MATKEY_COLOR_SPECULAR, colorTemp);
			mat->specular = glm::vec3(colorTemp.r, colorTemp.g, colorTemp.b);
			material->Get(AI_MATKEY_SHININESS, shininess);
			mat->shininess = shininess;

			//textures
			mat->ambientTextures = loadMaterialTextures(material, aiTextureType_AMBIENT, TEXTURE_TYPE::AMBIENT);
			mat->diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_TYPE::DIFFUSE);
			mat->specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_TYPE::SPECULAR);

			materialCache.insert({ materialIndex, mat });
		}		
	}
	return std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices, mat);
}

std::vector<std::shared_ptr<Texture>> SceneLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TEXTURE_TYPE tname)
{
	aiString str;
	std::vector<std::shared_ptr<Texture>> ret;
	unsigned textureCount = mat->GetTextureCount(type);
	for (size_t i = 0; i < textureCount; i++)
	{		
		mat->GetTexture(type, i, &str);
		auto cacheIter = textureCache.find(str.C_Str());
		if (cacheIter != textureCache.cend())
		{
			ret.push_back(cacheIter->second);
		}
		else {
			auto t = std::make_shared<Texture>(getDirectory() + "/" + str.C_Str(), tname);
			textureCache.insert({ str.C_Str(), t });
			ret.push_back(t);
		}		
	}
	return ret;
}

glm::mat4x4 SceneLoader::transformaiMatrix4x4(const aiMatrix4x4& aimatrix)
{
	glm::mat4 glmatrix;
	glmatrix[0][0] = aimatrix.a1; glmatrix[0][1] = aimatrix.b1; glmatrix[0][2] = aimatrix.c1; glmatrix[0][3] = aimatrix.d1;
	glmatrix[1][0] = aimatrix.a2; glmatrix[1][1] = aimatrix.b2; glmatrix[1][2] = aimatrix.c2; glmatrix[1][3] = aimatrix.d2;
	glmatrix[2][0] = aimatrix.a3; glmatrix[2][1] = aimatrix.b3; glmatrix[2][2] = aimatrix.c3; glmatrix[2][3] = aimatrix.d3;
	glmatrix[3][0] = aimatrix.a4; glmatrix[3][1] = aimatrix.b4; glmatrix[3][2] = aimatrix.c4; glmatrix[3][3] = aimatrix.d4;
	return glmatrix;
}

glm::vec3 SceneLoader::transformVec3(const aiVector3D& aivec)
{
	return glm::vec3{ aivec.x, aivec.y, aivec.z };
}