#pragma once
#include <memory>


enum PostProcessMode
{
	GrayScale,
	Bloom,
	Shadow,
	RGBSpliter
};

class Quad
{
public:
	Quad()
	{
		/*std::vector<float> vertices = { -1.0f, -1.0f,  0.0f, 0.0f,
										 1.0f, -1.0f,  1.0f, 0.0f,									
										 1.0f,  1.0f,  1.0f, 1.0f 
										-1.0f,  1.0f,  0.0f, 1.0f,										
		};
		std::vector<unsigned> indices = { 0, 1, 2, 2, 3, 0 };*/
		std::vector<float> vertices = { -1.0f, 1.0f, 0.0f, 1.0f,
										-1.0f, -1.0f, 0.0f, 0.0f,
										 1.0f, -1.0f, 1.0f, 0.0f,										
										-1.0f, 1.0f, 0.0f, 1.0f,
										 1.0f, -1.0f, 1.0f, 0.0f,
										 1.0f, 1.0f, 1.0f, 1.0f
		};

		m_VAO = std::make_shared<VertexArray>();
		m_VBO = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
		//m_IBO = std::make_shared<IndexBuffer>(indices.data(), 6);
		layout = std::make_shared<VertexDataLayout>();
		layout->push<float>(2);
		layout->push<float>(2);
		m_VAO->bind();
		m_VAO->AddBuffer(*m_VBO, *layout);
		m_VAO->unbind();
	}

	void bind()
	{
		m_VAO->bind();
		//m_IBO->bind();
	}

	void unbind()
	{
		//m_IBO->unbind();
		m_VAO->unbind();
	}
private:
	std::shared_ptr<VertexArray>  m_VAO;
	std::shared_ptr<IndexBuffer>  m_IBO;
	std::shared_ptr<VertexBuffer> m_VBO;
	std::shared_ptr<VertexDataLayout> layout;
};

class PostProcess
{
public:
	PostProcess(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, PostProcessMode mode) :
		name(name),
		mode(mode),
		program(std::make_shared<Shader>(vertexShaderPath, fragmentShaderPath)),
		m_FBO(std::make_shared<FrameBuffer>(1, 1))
	{ }

	void bind() const
	{
		program->bind();
		m_FBO->bind();
	}

	void unbind() const
	{
		m_FBO->unbind();
		program->unbind();
	}

	void draw(unsigned pipelineID) const
	{
		program->setInt("pipeLineTexture", pipelineID);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void resetFBO(unsigned viewporX, unsigned viewportY)
	{
		m_FBO->resize(viewporX, viewportY);
	}

	std::shared_ptr<FrameBuffer> getFBO() const { return m_FBO; };
private:
	std::string name;
	PostProcessMode mode;
	std::shared_ptr<Shader> program;
	std::shared_ptr<FrameBuffer> m_FBO;
};