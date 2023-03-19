#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../Buffer/IndexBuffer.h"
#include "../Buffer//VertexBuffer.h"
#include "../Buffer/VertexArray.h"
#include "../Buffer/VertexDataLayout.h"
#include "../material/Material.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	//uvs支持 vec1 vec2 vec3,最多8个通道
	std::vector<glm::vec3> uvs;
};

class Mesh
{
public:
	Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, std::shared_ptr<Material> mat) :
		name(name),
		vertices(vertices), 
		indices(indices),
		vao(VertexArray()), 
		vbo(VertexBuffer(vertices.data(), sizeof(Vertex)* vertices.size())), 
		ibo(indices.data(), indices.size()), 
		layout(VertexDataLayout()),
		material(mat)
	{
		vao.bind();
		layout.push<float>(3);
		layout.push<float>(3);
		layout.push<float>(2);
		vao.AddBuffer(vbo, layout);
		vao.unbind();
	}
	~Mesh() {}
private:
	std::string name;
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	VertexArray vao;
	VertexBuffer vbo;
	IndexBuffer ibo;
	VertexDataLayout layout;
	std::shared_ptr<Material> material;
};