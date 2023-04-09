#include "kspch.h"
#include "../../../Buffer/IndexBuffer.h"
#include "../../../Buffer/VertexArray.h"
#include "../../../Buffer/VertexDataLayout.h"
#include "../../../Buffer/VertexBuffer.h"
#include "Quad.h"

Quad::Quad()
{
	{
		std::vector<float> vertices = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		std::vector<unsigned> indices = { 0, 1, 2, 0, 2, 3 };

		m_VAO = std::make_shared<VertexArray>();
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		m_IBO = std::make_shared<IndexBuffer>(indices.data(), 6);
		layout = std::make_shared<VertexDataLayout>();
		layout->push<float>(2);
		layout->push<float>(2);
		m_VAO->bind();
		m_VAO->AddBuffer(*m_VBO, *layout);
		m_VAO->unbind();
	}
}

Quad::~Quad()
{
	unbind();
}

void Quad::bind()
{
	m_VAO->bind();
	m_IBO->bind();
}

void Quad::unbind()
{
	m_IBO->unbind();
	m_VAO->unbind();
}