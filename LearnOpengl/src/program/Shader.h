#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Buffer/FrameBuffer.h"

enum ShaderType
{
	WireFrame,
	BlinnPhong,
	PBR,
	PBR2,
	Depth,
	Normal,
	Grid,
	EnvironmentMapReflect,
	EnvironmentMapRefract,
	visualNormal,
	ShadowMap,
	BlinnPhongCastShadow,
	GridCastShadow,
	GrayScalize,
	GlitchRGBSplit,
	Inversion,
	NuClear,
	EdgeDetection,
	Base
};

enum ShaderCategory
{
	NormalShader,
	PostProcessShader
};

class Shader
{
public:
	Shader(const std::vector<std::string>& paths);
	virtual ~Shader();
	virtual void bind() const;
	virtual void unbind() const;

	virtual void update(float ts) { };
	virtual std::shared_ptr<FrameBuffer> getFBO() const { return nullptr; };
	virtual std::shared_ptr<FrameBuffer> getPrevFBO() const { return nullptr; };

	void setVec4(const std::string& name, const glm::vec4& v4) const;
	void setVec3(const std::string& name, const glm::vec3& v3) const;
	void setVec2(const std::string& name, const glm::vec2& v2) const;
	void setInt(const std::string& name, int iv) const;
	void setFloat(const std::string& name, float fv) const;
	void setMatrix44(const std::string& name, const glm::mat4x4& mat44) const;
	void setMatrix33(const std::string& name, const glm::mat3x3& mat33) const;

	ShaderCategory getType() const { return type; };
private:
	int getUniformLocation(const std::string& name) const;
	inline void printUniformError(const std::string& name) const
	{
		std::cout << "[set Uniform]:" << "UNIFORM " << name << " Not found!" << std::endl;
	}
protected:
	unsigned m_RendererID;
	unsigned compileShader(const std::string& path, unsigned shaderType);
	mutable std::unordered_map<std::string, int> uniformLocationCaches;
	ShaderCategory type;
};
