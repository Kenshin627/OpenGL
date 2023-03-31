#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../sceneLoader/sceneLoader.h"
#include "../camera/camera.h"
#include "../light/directionLight.h"
#include "../program/Shader.h"
#include "../Buffer/FrameBuffer.h"
//#include "../program/postProcess/PostProcess.h"
#include "../mesh/BasicMeshes/Quad/Quad.h"

#include "../mesh/BasicMeshes/SkyBox/SkyBox.h"
#include "../Buffer/ShadowFrameBuffer.h"

class GridMesh;

enum RenderMode
{
	_WireFrame,
	_BlinnPhong,
	_Depth,
	_Normal
};

class X_Renderer
{
public:
	X_Renderer();
	~X_Renderer();
	void Render(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts);
	void RenderShadow(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts);
	void Recursivedraw(const std::shared_ptr<Node>& node);
	const std::shared_ptr<Camera> getCamera() const { return camera; };
	void resizeFBO(unsigned width, unsigned height);
	void clear();
	unsigned getFrameBufferTextureID() const { return outputTextureID; };
	void setRenderMode(RenderMode _mode) { mode = _mode; };
	RenderMode getRenderMode() const { return mode; };
	void CompileShaders();
	glm::vec3& getWireFrameColor() { return wireFrameColor; };
	const std::vector<std::shared_ptr<DirectionLight>>& getLights() const { return lights; };
	std::shared_ptr<Shader> getShader(ShaderType type) const { return shaderLib.find(type)->second; };
private:
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<DirectionLight>> lights;
	std::shared_ptr<FrameBuffer> m_FBO;
	std::shared_ptr<FrameBuffer> prevFBO;
	std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaderLib;
	glm::vec4 clearColor;
	RenderMode mode;
	glm::vec3 wireFrameColor;
	std::shared_ptr<GridMesh> grid;
	Quad quad;
	SkyBox skybox;
	std::unordered_map<std::string, float> refractiveIndex;
	unsigned outputTextureID;
	std::shared_ptr<ShadowFrameBuffer> m_ShadowFBO;
	bool enableShadows;
	bool visiualNormal;
	std::shared_ptr<Shader> postProcess;
};