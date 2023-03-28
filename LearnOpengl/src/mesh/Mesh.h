#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "../Buffer/IndexBuffer.h"
#include "../Buffer//VertexBuffer.h"
#include "../Buffer/VertexArray.h"
#include "../Buffer/VertexDataLayout.h"
#include "../material/Material.h"

class Mesh
{
public:
	Mesh(const std::string& name, const std::vector<float>& vertices, const std::vector<unsigned>& indices, std::shared_ptr<Material> mat, const VertexDataLayout& layout) :
		name(name),
		vertices(vertices), 
		indices(indices),
		vao(VertexArray()), 
		vbo(VertexBuffer(vertices.data(), sizeof(float)* vertices.size())), 
		ibo(indices.data(), indices.size()), 
		layout(layout),
		material(mat),
		modelMatrix(glm::identity<glm::mat4x4>())
	{
		vao.bind();
		vao.AddBuffer(vbo, layout);
		vao.unbind();
	}
	void bind() const
	{
		vao.bind();
		ibo.bind();
	}

	void unbind() const 
	{ 
		ibo.unbind();
		vao.unbind();
	}
	const std::shared_ptr<Material>& getMaterial() const { return material; };
	const unsigned indicesCount() const { return ibo.indicesCount(); };
	const std::string getName() const { return name; };
	const glm::mat4x4& getModelMatrix() const { return modelMatrix; };
	virtual ~Mesh() {}
protected:
	std::string name;
	std::vector<float> vertices;
	std::vector<unsigned> indices;

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	VertexDataLayout layout;
	std::shared_ptr<Material> material;
	glm::mat4x4 modelMatrix;
};