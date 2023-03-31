#include "Grid.h"

GridMesh::GridMesh(float size) :Mesh("grid",
	{
	   -size, 0, -size,  0, 1, 0,  0, 1,
	   -size, 0,  size,  0, 1, 0,  0, 0,
		size, 0,  size,  0, 1, 0,  1, 0,
		size, 0, -size,  0, 1, 0,  1, 1
	},
	{ 0, 1, 2, 0, 2, 3 },
	nullptr,
	VertexDataLayout().push<float>(3).push<float>(3).push<float>(2)
) { }
