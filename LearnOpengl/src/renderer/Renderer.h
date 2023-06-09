#pragma once
#include "../sceneLoader/sceneLoader.h"
#include "../camera/camera.h"
#include "../light/directionLight.h"
#include "../program/Shader.h"
#include "../Buffer/FrameBuffer.h"
#include "../mesh/BasicMeshes/Quad/Quad.h"
#include "../mesh/BasicMeshes/SkyBox/SkyBox.h"
#include "../Buffer/ShadowFrameBuffer.h"
#include "../light/pointLight.h"
#include "../ibl/IBL.h"

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
	void Render(const SceneGraph& sceneGraph, const glm::ivec2& viewport, float ts);
	void RenderShadow(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts);
	void Recursivedraw(const std::shared_ptr<Node>& node);
	const std::shared_ptr<Camera> getCamera() const { return camera; };
	void resizeFBO(unsigned width, unsigned height);
	void clear();
	unsigned getFrameBufferTextureID() const { return outputTextureID; };
	void setRenderMode(RenderMode _mode) { mode = _mode; };
	RenderMode getRenderMode() const { return mode; };
	void CompileShaders();
	unsigned getSkyBoxTexture() const { return skybox.getTextureID(); };
	glm::vec3& getWireFrameColor() { return wireFrameColor; };
	const std::vector<std::shared_ptr<DirectionLight>>& getLights() const { return directionLights; };
	const std::vector<std::shared_ptr<PointLight>>& getPointLights() const { return pointLights; };
	std::shared_ptr<Shader> getShader(ShaderType type) const { return shaderLib.find(type)->second; };
	const std::shared_ptr<IBL>& getIBL() const { return ibl; };
	void setSkyBoxTexture(unsigned textureID) { skybox.setTextureID(textureID); };
private:
	std::shared_ptr<Camera> camera;
	std::vector<std::shared_ptr<DirectionLight>> directionLights;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::shared_ptr<FrameBuffer> m_FBO;
	std::shared_ptr<FrameBuffer> prevFBO;
	std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaderLib;
	glm::vec4 clearColor;
	RenderMode mode;
	glm::vec3 wireFrameColor;
	std::shared_ptr<GridMesh> grid;
	Quad quad;
	SkyBox skybox;
	unsigned outputTextureID;
	std::shared_ptr<ShadowFrameBuffer> m_ShadowFBO;
	bool enableShadows;
	bool visiualNormal;
	std::shared_ptr<Shader> postProcess;
	std::shared_ptr<IBL> ibl;
};