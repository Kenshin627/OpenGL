#include "TorusKnot.h"

TorusKnot::TorusKnot(const std::string& name, float radius, float tube, unsigned tubelarSegments, unsigned radialSegments, float p, float q, std::shared_ptr<Material> mat) :
	Mesh(name, {}, {}, mat, VertexDataLayout().push<float>(3).push<float>(3).push<float>(2)),
	radius(radius),
	tube(tube),
	tubelarSegments(tubelarSegments),
	radialSegments(radialSegments),
	p(p),
	q(q) 
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

TorusKnot::~TorusKnot() {}

void TorusKnot::ConstructorVertex()
{
	//reverse storage
	vertices.reserve((tubelarSegments + 1) * (radialSegments + 1) * 8);
	indices.reserve(tubelarSegments * radialSegments * 6);

	glm::vec3 p1, p2, T, B, N, position, normal, uv;
	for (float i = 0.0f; i <= tubelarSegments; ++i) {
		float u = i / tubelarSegments * p * glm::two_pi<float>();
		p1 = calculatePositionOnCurve(u, p, q, radius);
		p2 = calculatePositionOnCurve(u + 0.01, p, q, radius);

		// calculate orthonormal basis
		T = p2 - p1;
		N = p2 + p1;
		B = glm::cross(T, N);
		N = glm::cross(B, T);
		B = glm::normalize(B);
		N = glm::normalize(N);
		for (float j = 0; j <= radialSegments; ++j) {
			float v = j / radialSegments * glm::two_pi<float>();
			float cx = -tube * cosf(v);
			float cy =  tube * sinf(v);
			
			//position
			position = { p1.x + (cx * N.x + cy * B.x), p1.y + (cx * N.y + cy * B.y), p1.z + (cx * N.z + cy * B.z) };
			vertices.push_back(position.x);
			vertices.push_back(position.y);
			vertices.push_back(position.z);
			
			//normal
			normal = position - p1;
			normal = glm::normalize(normal);
			vertices.push_back(normal.x);
			vertices.push_back(normal.y);
			vertices.push_back(normal.z);

			//uv
			vertices.push_back(i / tubelarSegments);
			vertices.push_back(j / radialSegments);
		}
	}

	//generate indices
	for (float j = 1.0f; j <= tubelarSegments; j++) {
		for (float i = 1.0f; i <= radialSegments; i++) {
			// indices
			float a = (radialSegments + 1) * (j - 1) + (i - 1);
			float b = (radialSegments + 1) * j + (i - 1);
			float c = (radialSegments + 1) * j + i;
			float d = (radialSegments + 1) * (j - 1) + i;
			// faces
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(d);

			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(d);
		}
	}
}

glm::vec3 TorusKnot::calculatePositionOnCurve(float u, float p, float q, float radius) const
{
	glm::vec3 ret{};

	float cu = cosf(u);
	float su = sinf(u);
	float quOverp = q / p * u;
	float cs = cosf(quOverp);

	ret.x = radius * (2 + cs) * 0.5f * cu;
	ret.y = radius * (2 + cs) * su * 0.5f;
	ret.z = radius * sinf(quOverp) * 0.5f;

	return ret;
}