#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../Buffer/IndexBuffer.h"
#include "../Buffer//VertexBuffer.h"
#include "../Buffer/VertexArray.h"
#include "../Buffer/VertexDataLayout.h"
#include "../material/Material.h"

class Mesh
{
public:
	Mesh(const std::string& name, const std::vector<float>& vertices, const std::vector<unsigned>& indices, std::shared_ptr<Material> mat, const VertexDataLayout& layout);
	const std::shared_ptr<Material>& getMaterial() const { return material; };
	const unsigned indicesCount() const { return ibo->indicesCount(); };
	const std::string getName() const { return name; };
	const glm::mat4x4& getModelMatrix() const { return modelMatrix; };
	void setModelMatrix(const glm::mat4x4& mat) { modelMatrix = mat; };
	void setMaterial(std::shared_ptr<Material> mat);
	virtual ~Mesh() { }
	virtual void draw();
protected:
	std::string name;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	std::unique_ptr<VertexArray> vao;
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ibo;
	VertexDataLayout layout;
	std::shared_ptr<Material> material;
	glm::mat4x4 modelMatrix;
	bool isIndexed;
};   