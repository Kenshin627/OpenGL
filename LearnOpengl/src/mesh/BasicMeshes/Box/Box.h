#pragma once
#include <string>
#include "../../Mesh.h"


class BoxMesh:public Mesh
{
public:
	BoxMesh(const std::string& name, float w, float h, float d);
private:	
	float width;
	float height;
	float depth;
};