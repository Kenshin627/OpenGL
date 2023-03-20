#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../sceneLoader/sceneLoader.h"
#include "../camera/camera.h"
#include "../light/directionLight.h"
#include "../program/Shader.h"


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
	void Render(const SceneGraph& sceneGraph, RenderMode mode);
	void Recursivedraw(const std::shared_ptr<Node>& node, const Shader& p);
private:
	std::shared_ptr<Camera> camera;
	std::vector<DirectionLight> lights;
};