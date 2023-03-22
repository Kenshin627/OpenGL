#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../sceneLoader/sceneLoader.h"
#include "../camera/camera.h"
#include "../light/directionLight.h"
#include "../program/Shader.h"
#include "../Buffer/FrameBuffer.h"

enum RenderMode
{
	wireFrame,
	BlinnPhong,
	PBR,
	Depth
};

class X_Renderer
{
public:
	X_Renderer();
	~X_Renderer();
	void Render(const SceneGraph& sceneGraph, RenderMode mode, const glm::vec2& viewport);
	void Recursivedraw(const std::shared_ptr<Node>& node, const Shader& p);
	std::shared_ptr<Camera> getCamera() { return camera; };
	void buildFBO(const glm::vec2& viewport);
	void clear();
	unsigned getFrameBufferTextureID() const { return m_FBO->GetTextureID(); };
private:
	std::shared_ptr<Camera> camera;
	std::vector<DirectionLight> lights;
	std::unique_ptr<FrameBuffer> m_FBO;
	glm::vec4 clearColor;
};