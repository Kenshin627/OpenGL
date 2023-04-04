#pragma once
#include <string>
#include <memory>
#include <vector>
#include "../../../Buffer/VertexArray.h"
#include "../../../Buffer/VertexBuffer.h"
#include "../../../Buffer/IndexBuffer.h"
#include "../../../Buffer/VertexDataLayout.h"
#include "../../../program/Shader.h"
#include "../../../camera/camera.h"
class SkyBox
{
public:
	SkyBox(const std::vector<std::string>& faces, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void draw(const Camera& camera, unsigned slot = 0);
	void bind();
	void unbind();
	unsigned getTextureID() const;
	unsigned getFormat(unsigned nrChannels) const;
	void setTextureID(unsigned id);
private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<Shader> program;
	std::unique_ptr<VertexDataLayout> m_Layout;
	unsigned m_TextureID;
};