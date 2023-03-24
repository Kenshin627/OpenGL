#include <vector>
#include "../Mesh.h"

class Plane :public Mesh
{
public:
	Plane(const std::string& name) :Mesh(
		name, 
		std::vector<Vertex>{
			Vertex{ glm::vec3{-100, 0, -100}, glm::vec3{0, 1, 0}, glm::vec2{0,1} }, 
			Vertex{ glm::vec3{-100, 0, 100}, glm::vec3{0, 1, 0}, glm::vec2{0,0} },
			Vertex{ glm::vec3{100, 0, 100}, glm::vec3{0, 1, 0}, glm::vec2{1,0} }, 
			Vertex{ glm::vec3{100, 0, -100}, glm::vec3{0, 1, 0}, glm::vec2{1,1} }, 
		}, 
		std::vector<unsigned>{ 0, 1, 2, 0, 2, 3 }, 
		nullptr
	) 
	{
		
	}
private:
};