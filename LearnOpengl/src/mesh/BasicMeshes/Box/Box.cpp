#include "kspch.h"
#include "Box.h"

BoxMesh::BoxMesh(const std::string& name, float w, float h, float d, std::shared_ptr<Material> mat) :Mesh(name,
	{   
        //ccw layout 3 3 2        
        -w / 2, -h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,//back
         w / 2, -h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
         w / 2,  h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
         w / 2,  h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -w / 2,  h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -w / 2, -h / 2, -d / 2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -w / 2, -h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,//front
         w / 2, -h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
         w / 2,  h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
         w / 2,  h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        -w / 2,  h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,
        -w / 2, -h / 2,  d / 2, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

        -w / 2,  h / 2,  d / 2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//left
        -w / 2,  h / 2, -d / 2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -w / 2, -h / 2, -d / 2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -w / 2, -h / 2, -d / 2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -w / 2, -h / 2,  d / 2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -w / 2,  h / 2,  d / 2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

         w / 2,  h / 2,  d / 2,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//right
         w / 2,  h / 2, -d / 2,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         w / 2, -h / 2, -d / 2,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         w / 2, -h / 2, -d / 2,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         w / 2, -h / 2,  d / 2,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         w / 2,  h / 2,  d / 2,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -w / 2, -h / 2, -d / 2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,//bottom
         w / 2, -h / 2, -d / 2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
         w / 2, -h / 2,  d / 2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         w / 2, -h / 2,  d / 2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -w / 2, -h / 2,  d / 2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -w / 2, -h / 2, -d / 2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
  
         w / 2,  h / 2,  d / 2, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,//top
         w / 2,  h / 2, -d / 2, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -w / 2,  h / 2, -d / 2, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -w / 2,  h / 2, -d / 2, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -w / 2,  h / 2,  d / 2, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
         w / 2,  h / 2,  d / 2, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f
	}, 
	{ }, 
    mat,
	VertexDataLayout().push<float>(3).push<float>(3).push<float>(2)),
	width(w),
	height(h),
	depth(d) {}