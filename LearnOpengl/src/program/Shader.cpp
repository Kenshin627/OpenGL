#include "kspch.h"
#include <glad/gl.h>
#include "Shader.h"

Shader::Shader(const std::vector<std::string>& paths):m_RendererID(0)
{
	int success;
	std::array<char, 512> msg;
	m_RendererID = glCreateProgram();

	unsigned vertexID = compileShader(paths[0], GL_VERTEX_SHADER);
	unsigned fragmentID = compileShader(paths[1], GL_FRAGMENT_SHADER);
	unsigned geometryID = 0;
	if (paths.size() == 3)
	{
		unsigned geometryID = compileShader(paths[2], GL_GEOMETRY_SHADER);
		glAttachShader(m_RendererID, geometryID);
	}
	
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
	if (geometryID != 0)
	{
		glDeleteShader(geometryID);
	}
	type = ShaderCategory::NormalShader;
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
	
unsigned Shader::compileShader(const std::string& path, unsigned shaderType)
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
			std::cout << "ERROR::SHADER::" << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : shaderType == GL_FRAGMENT_SHADER? "FRAGMENT" : "GEOMETRY") << "::COMPILATION_FAILED\n" << msg.data() << std::endl;
		}
		return shaderID;
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:" << e.what() << std::endl;
		return -1;
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

void Shader::setMatrix44(const std::string& name, const glm::mat4x4& mat44) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat44));
		return;
	}
	printUniformError(name);
}

void Shader::setMatrix33(const std::string& name, const glm::mat3x3& mat33) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat33));
		return;
	}
	printUniformError(name);
}

void Shader::setVec4(const std::string& name, const glm::vec4& v4) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4fv(location, 1, glm::value_ptr(v4));
		return;
	}
	printUniformError(name);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v3) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3fv(location, 1, glm::value_ptr(v3));
		return;
	}
	printUniformError(name);
}

void Shader::setVec2(const std::string& name, const glm::vec2& v2) const
{
	int location = getUniformLocation(name);
	if (location != -1) 
	{
		glUniform2fv(location, 1, glm::value_ptr(v2));
		return;
	}
	printUniformError(name);
}

void Shader::setInt(const std::string& name, int iv) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1i(location, iv);
		return;
	}
	printUniformError(name);
}

void Shader::setFloat(const std::string& name, float fv) const
{
	int location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1f(location, fv);
		return;
	}
	printUniformError(name);
}