#pragma once
#include <string>
#include "../../Mesh.h"


class BoxMesh:public Mesh
{
public:
	BoxMesh(const std::string& name, float w, float h, float d, std::shared_ptr<Material> mat);
private:	
	float width;
	float height;
	float depth;
};