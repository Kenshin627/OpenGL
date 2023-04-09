#pragma once
#include "../../dependencies/ENTT/entt.h"

class Scene
{
public:
	Scene();
	~Scene();
private:
	entt::registry m_Registry;
};