#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../sceneLoader/sceneLoader.h"
#include "../camera/camera.h"
#include "../light/directionLight.h"
#include "../program/Shader.h"
#include "../Buffer/FrameBuffer.h"
#include "postProcess/PostProcess.h"
#include "../mesh/BasicMeshes/Quad/Quad.h"
#include "../mesh/BasicMeshes/Grid/Grid.h"
#include "../mesh/BasicMeshes/SkyBox/SkyBox.h"
#include "../Buffer/ShadowFrameBuffer.h"

enum ShaderType
{
	WireFrame,
	BlinnPhong,
	PBR,
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
	EdgeDetection
};

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
	void Recursivedraw(const std::shared_ptr<Node>& node, std::shared_ptr<Shader> shader = nullptr);
	std::shared_ptr<Camera> getCamera() { return camera; };
	void resizeFBO(unsigned width, unsigned height);
	void clear();
	unsigned getFrameBufferTextureID() const { return outputTextureID; };
	void setRenderMode(RenderMode _mode) { mode = _mode; };
	RenderMode getRenderMode() const { return mode; };
	void compileShaders();
	glm::vec3& getWireFrameColor() { return wireFrameColor; };
private:
	std::shared_ptr<Camera> camera;
	std::vector<DirectionLight> lights;
	std::shared_ptr<FrameBuffer> m_FBO;
	std::shared_ptr<FrameBuffer> prevFBO;
	std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaderLib;
	glm::vec4 clearColor;
	RenderMode mode;
	glm::vec3 wireFrameColor;
	GridMesh grid;
	Quad quad;
	SkyBox skybox;
	std::unordered_map<std::string, float> refractiveIndex;
	unsigned outputTextureID;
	std::shared_ptr<ShadowFrameBuffer> shadow_FBO;
};