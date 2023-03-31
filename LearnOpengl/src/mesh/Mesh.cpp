#include <glad/gl.h>
#include "Mesh.h"

Mesh::Mesh(const std::string& name, const std::vector<float>& vertices, const std::vector<unsigned>& indices, std::shared_ptr<Material> mat, const VertexDataLayout& layout):
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

void Mesh::draw()
{
	material->bind();
	vao->bind();
	if (isIndexed)
	{
		ibo->bind();
	}

	material->setUniforms(*this);

	if (isIndexed)
	{
		glDrawElements(GL_TRIANGLES, indicesCount(), GL_UNSIGNED_INT, (const void*)0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	if (isIndexed)
	{
		ibo->unbind();
	}
	vao->unbind();
	material->unbind();
}


void Mesh::setMaterial(std::shared_ptr<Material> mat)
{
	material = mat;
}