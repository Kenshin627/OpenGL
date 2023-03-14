#pragma once

#include <iostream>
#include <fstream>
#include <glad/gl.h>
#include <string>
#include <sstream>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath) 
	{
		compileShader(vertexPath, GL_VERTEX_SHADER);
		compileShader(fragmentPath, GL_FRAGMENT_SHADER);
	}

	void use()
	{
		glUseProgram(ID);
	}


private:
	unsigned ID;
	void compileShader(const std::string& path, GLenum shaderType);
};

void Shader::compileShader(const std::string& path, GLenum shaderType)
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
		glCreateShader(shaderType);
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}