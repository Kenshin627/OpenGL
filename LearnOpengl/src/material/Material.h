#pragma once

#include "../program/Shader.h"
#include "../renderer/Renderer.h"

class X_Renderer;

class Material
{
public:
	Material(ShaderType type, const X_Renderer& renderer);
	virtual ~Material();
	virtual void setUniforms(const Mesh& mesh) { };
	void bind() const { program->bind(); };
	void unbind() const { program->unbind(); };
public:
	ShaderType type;		
protected:
	const X_Renderer& renderer;
	std::shared_ptr<Shader> program;
};