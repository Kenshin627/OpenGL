#include <vector>
#include <memory>

#include "../../../Buffer/IndexBuffer.h"
#include "../../../Buffer/VertexArray.h"
#include "../../../Buffer/VertexDataLayout.h"
#include "../../../Buffer/VertexBuffer.h"
#include "Grid.h"

GridMesh::GridMesh(float size)
{
	{
		std::vector<float> vertices = {
			-size, 0, -size,  0, 1, 0,  0, 1,
			-size, 0,  size,  0, 1, 0,  0, 0,
			 size, 0,  size,  0, 1, 0,  1, 0,
			 size, 0, -size,  0, 1, 0,  1, 1,
		};

		std::vector<unsigned> indices = { 0, 1, 2, 0, 2, 3 };

		m_VAO = std::make_shared<VertexArray>();
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		m_IBO = std::make_shared<IndexBuffer>(indices.data(), 6);
		layout = std::make_shared<VertexDataLayout>();
		layout->push<float>(3);
		layout->push<float>(3);
		layout->push<float>(2);
		m_VAO->bind();
		m_VAO->AddBuffer(*m_VBO, *layout);
		m_VAO->unbind();
	}
}

GridMesh::~GridMesh()
{
	unbind();
}

void GridMesh::bind()
{
	m_VAO->bind();
	m_IBO->bind();
}

void GridMesh::unbind()
{
	m_IBO->unbind();
	m_VAO->unbind();
}