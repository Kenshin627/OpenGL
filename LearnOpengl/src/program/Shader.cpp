#include <glad/gl.h>
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
	glUseProgram(0);
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

int Shader::getUniformLocation(const std::string& name) const
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

void Shader::setVec4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, v1, v2, v3, v4);
		return;
	}
	printUniformError(name);
}

void Shader::setVec3(const std::string& name, float v1, float v2, float v3) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3f(location, v1, v2, v3);
		return;
	}
	printUniformError(name);
}

void Shader::setVec2(const std::string& name, float v1, float v2) const
{
	int location = getUniformLocation(name);
	if (location != -1) 
	{
		glUniform2f(location, v1, v2);
		return;
	}
	printUniformError(name);
}

void Shader::setInt(const std::string& name, int v1) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1i(location, v1);
		return;
	}
	printUniformError(name);
}

void Shader::setMatrix44(const std::string& name, const glm::mat4x4& mat) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		return;
	}
	printUniformError(name);
}

void Shader::setMatrix33(const std::string& name, const glm::mat3x3& mat) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		return;
	}
	printUniformError(name);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec3) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3fv(location, 1, glm::value_ptr(vec3));
		return;
	}
	printUniformError(name);
}

void Shader::setFloat(const std::string& name, float v1) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1f(location, v1);
		return;
	}
	printUniformError(name);
}