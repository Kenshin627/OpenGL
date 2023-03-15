#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath):m_RendererID(0)
{
	int success;
	std::array<char, 512> msg;
	unsigned vertexID = compileShader(vertexPath, GL_VERTEX_SHADER);
	unsigned fragmentID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	m_RendererID = glCreateProgram();
	glAttachShader(m_RendererID, vertexID);
	glAttachShader(m_RendererID, fragmentID);
	glLinkProgram(m_RendererID);
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_RendererID, 512, nullptr, msg.data());
		std::cout << "ERROR:PROGRAM:: LINK_FAILED\n" << msg.data() << std::endl;
	}
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::unbind() const
{
	glUseProgram(m_RendererID);
}

unsigned Shader::compileShader(const std::string& path, GLenum shaderType)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
		const char* vShaderCode = shaderCode.c_str();
		unsigned shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &vShaderCode, nullptr);
		glCompileShader(shaderID);
		int success;
		std::array<char, 512> msg;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, nullptr, msg.data());
			std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << msg.data() << std::endl;
		}
		return shaderID;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:" << e.what() << std::endl;
	}
}

int Shader::getUniformLocation(std::string& name) const
{
	auto cacheLocation = uniformLocationCaches.find(name);
	if (cacheLocation != uniformLocationCaches.cend())
	{
		return cacheLocation->second;
	}
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location != -1)
	{
		uniformLocationCaches.insert({ name, location });
	}
	return location;
}

void Shader::setUniform4f(std::string& name, float v1, float v2, float v3, float v4) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, v1, v2, v3, v4);
		return;
	}
	printUniformError(name);
}

void Shader::setUniform3f(std::string& name, float v1, float v2, float v3) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3f(location, v1, v2, v3);
		return;
	}
	printUniformError(name);
}

void Shader::setUniform2f(std::string& name, float v1, float v2) const
{
	int location = getUniformLocation(name);
	if (location != -1) 
	{
		glUniform2f(location, v1, v2);
		return;
	}
	printUniformError(name);
}

void Shader::setUniform1i(std::string& name, int v1) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1i(location, v1);
		return;
	}
	printUniformError(name);
	{

	}
}