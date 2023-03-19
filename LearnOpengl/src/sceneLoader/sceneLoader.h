#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../mesh/Mesh.h"
#include "../material/Material.h"

struct Node
{
	Node():parent(nullptr),nodeName("empty"),modelMatrix(glm::identity<glm::mat4x4>()) {};
	~Node() {};
	std::shared_ptr<Node> parent;
	std::vector<std::shared_ptr<Node>> children;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::string nodeName;
	glm::mat4x4 modelMatrix;
};

struct SceneGraph
{
	std::vector<std::shared_ptr<Node>> roots;
};

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader();
	std::shared_ptr<Node> loadModel(const std::string& path);
	static glm::mat4x4 transformaiMatrix4x4(const aiMatrix4x4& aimatrix);
	static glm::vec3   transformVec3(const aiVector3D& vec);
	const std::string& getDirectory() const { return directory; };
private:
	std::shared_ptr<Node> processNode(aiNode* node, const aiScene* scene, std::shared_ptr<Node> parent);
	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TEXTURE_TYPE tname);
private:
	Assimp::Importer importer;
	std::string directory;
	std::unordered_map<unsigned, std::shared_ptr<Material>> materialCache;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textureCache;
};