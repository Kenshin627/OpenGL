#include "Sphere.h"

Sphere::Sphere(const std::string& name, float radius, float widthSegments, float heightSegments, float phiStart, float phiLength, float thetaStart, float thetaLength, float uScale, float vScale, std::shared_ptr<Material> mat) :Mesh(name, {}, {}, mat, VertexDataLayout().push<float>(3).push<float>(3).push<float>(2)), radius(radius), widthSegments(widthSegments), heightSegments(heightSegments), thetaStart(thetaStart), thetaLength(thetaLength), phiStart(phiStart), phiLength(phiLength), uScale(uScale), vScale(vScale)
{
	ConstructorVertex();
	isIndexed = true;
	vbo = std::make_unique<VertexBuffer>(vertices.data(), sizeof(float) * vertices.size());
	ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
	vao = std::make_unique<VertexArray>();
	vao->bind();
	vao->AddBuffer(*vbo, layout);
	vao->unbind();
}

void Sphere::ConstructorVertex()
{
	widthSegments =  glm::max<float>(3.0f, widthSegments);
	heightSegments = glm::max<float>(2.0f, heightSegments);
	auto thetaEnd = glm::min<float>(thetaStart + thetaLength, glm::pi<float>());
	std::vector<std::vector<unsigned>> grid;

	//reverse storage
	vertices.reserve((widthSegments + 1) * (heightSegments + 1) * 8);
	grid.reserve(widthSegments * heightSegments);
	indices.reserve(widthSegments * heightSegments * 3);
	unsigned index = 0;

	// generate vertices, normals and uvs
	for (unsigned iy = 0; iy <= heightSegments; iy++) {
		std::vector<unsigned> verticesRow;
		float v = iy / heightSegments;
		// special case for the poles
		float uOffset = 0;
		if (iy == 0 && thetaStart == 0) {
			uOffset = 0.5 / widthSegments;
		}
		else if (iy == heightSegments && thetaEnd == glm::pi<float>()) {
			uOffset = -0.5 / widthSegments;
		}
		for (unsigned ix = 0; ix <= widthSegments; ix++) {
			float u = ix / widthSegments;
			float x = radius * cosf(phiStart + u * phiLength) * sinf(thetaStart + v * thetaLength);
			float y = radius * cosf(thetaStart + v * thetaLength);
			float z = -radius * sinf(phiStart + u * phiLength) * sinf(thetaStart + v * thetaLength);

			//position
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			
			//normal
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// uv
			vertices.push_back((u + uOffset) * uScale);
			vertices.push_back((1 - v) * vScale);

			verticesRow.push_back(index++);
		}
		grid.push_back(verticesRow);
	}

	// indices
	for (unsigned iy = 0; iy < heightSegments; iy++) {
		for (unsigned ix = 0; ix < widthSegments; ix++) {
			unsigned a = grid[iy][ix + 1];
			unsigned b = grid[iy][ix];
			unsigned c = grid[iy + 1][ix];
			unsigned d = grid[iy + 1][ix + 1];

			if (iy != 0 || thetaStart > 0)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);
			};
			if (iy != heightSegments - 1 || thetaEnd < glm::pi<float>())
			{				
				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}
}