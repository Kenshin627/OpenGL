#include "Box.h"

BoxMesh::BoxMesh(const std::string& name, float w, float h, float d) :Mesh(name,
	{   
        //ccw layout 3 3 2
        //back
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        //front 
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        //left
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        //right
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         //bottom
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        //top
        -1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f
	}, 
	{ }, 
	nullptr, 
	VertexDataLayout().push<float>(3).push<float>(3).push<float>(2)),
	width(w),
	height(h),
	depth(d) {}