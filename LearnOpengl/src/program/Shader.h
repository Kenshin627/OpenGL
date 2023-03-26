#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	~Shader();
	void bind() const;
	void unbind() const;

	void setVec4(const std::string& name, const glm::vec4& v4) const;
	void setVec3(const std::string& name, const glm::vec3& v3) const;
	void setVec2(const std::string& name, const glm::vec2& v2) const;
	void setInt(const std::string& name, int iv) const;
	void setFloat(const std::string& name, float fv) const;
	void setMatrix44(const std::string& name, const glm::mat4x4& mat44) const;
	void setMatrix33(const std::string& name, const glm::mat3x3& mat33) const;
private:
	int getUniformLocation(const std::string& name) const;
	inline void printUniformError(const std::string& name) const
	{
		std::cout << "[set Uniform]:" << "UNIFORM " << name << " Not found!" << std::endl;
	}
private:
	unsigned m_RendererID;
	unsigned compileShader(const std::string& path, unsigned shaderType);
	mutable std::unordered_map<std::string, int> uniformLocationCaches;
};
