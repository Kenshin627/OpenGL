#pragma once

#include <iostream>
#include <fstream>
#include <glad/gl.h>
#include <string>
#include <sstream>
#include <array>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();
	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setUniform3f(const std::string& name, float v1, float v2, float v3) const;
	void setUniform2f(const std::string& name, float v1, float v2) const;
	void setUniform1i(const std::string& name, int v1) const;
private:
	int getUniformLocation(const std::string& name) const;
	inline void printUniformError(const std::string& name) const
	{
		std::cout << "[set Uniform]:" << "UNIFORM" << name << "Not found!" << std::endl;
	}
private:
	unsigned m_RendererID;
	unsigned compileShader(const std::string& path, GLenum shaderType);
	mutable std::unordered_map<std::string, int> uniformLocationCaches;
};
