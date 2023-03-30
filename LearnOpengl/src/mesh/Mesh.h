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
	Mesh(const std::string& name, const std::vector<float>& vertices, const std::vector<unsigned>& indices, std::shared_ptr<Material> mat, const VertexDataLayout& layout) :
		name(name),
		vertices(vertices), 
		indices(indices),
		vao(std::make_unique<VertexArray>()),
		ibo(nullptr),
		vbo(std::make_unique<VertexBuffer>(vertices.data(), sizeof(float)* vertices.size())),
		layout(layout),
		material(mat),
		modelMatrix(glm::identity<glm::mat4x4>()),
		isIndexed(false)
	{
		if (!indices.empty())
		{
			isIndexed = true;
			ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
		}
		vao->bind();
		vao->AddBuffer(*vbo, layout);
		vao->unbind();
	}
	void bind() const
	{
		vao->bind();
		if (isIndexed)
		{
			ibo->bind();
		}
	}

	void unbind() const 
	{ 
		if (isIndexed)
		{
			ibo->unbind();
		}
		vao->unbind();
	}
	const std::shared_ptr<Material>& getMaterial() const { return material; };
	const unsigned indicesCount() const { return ibo->indicesCount(); };
	const std::string getName() const { return name; };
	const glm::mat4x4& getModelMatrix() const { return modelMatrix; };
	virtual ~Mesh() { }
	virtual void draw() const 
	{
		if (isIndexed)
		{
			glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, (const void*)0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		}
	}
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