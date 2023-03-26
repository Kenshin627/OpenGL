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

enum RenderMode
{
	wireFrame,
	BlinnPhong,
	PBR,
	Depth,
	Normal,
	grid
};

class X_Renderer
{
public:
	X_Renderer();
	~X_Renderer();
	void Render(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts);
	void Recursivedraw(const std::shared_ptr<Node>& node, const Shader& p);
	std::shared_ptr<Camera> getCamera() { return camera; };
	void buildFBO(const glm::vec2& viewport);
	void resizeFBO(unsigned width, unsigned height);
	void clear();
	unsigned getFrameBufferTextureID() const { return outputTextureID; };
	void setRenderMode(RenderMode _mode) { mode = _mode; };
	RenderMode getRenderMode() const { return mode; };
	void compileShaders();
	glm::vec3& getWireFrameColor() { return wireFrameColor; };
	void compilePostProcess();
private:
	std::shared_ptr<Camera> camera;
	std::vector<DirectionLight> lights;
	std::shared_ptr<FrameBuffer> m_FBO;
	std::shared_ptr<FrameBuffer> prevFBO;
	std::unordered_map<RenderMode, std::shared_ptr<Shader>> shaders;
	glm::vec4 clearColor;
	RenderMode mode;
	glm::vec3 wireFrameColor;
	Grid grid;
	std::unordered_map<PostProcessMode, std::shared_ptr<PostProcess>> postProcesses;
	Quad quad;
	SkyBox skybox;
	unsigned outputTextureID;
};