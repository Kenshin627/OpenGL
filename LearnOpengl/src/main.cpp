#include "window/window.h"
#include "sceneLoader/sceneLoader.h"

int main()
{
	unsigned SCR_WIDTH = 800;
	unsigned SCR_HEIGHT = 600;

	auto window = Window(SCR_WIDTH, SCR_HEIGHT, std::make_shared<X_Renderer>());

	SceneLoader loader;
	SceneGraph sceneGraph;	
	auto root = loader.loadModel("resource/models/nanosuit/nanosuit.obj");
	if (root)
	{
		sceneGraph.roots.push_back(root);
	}
	window.renderLoop(sceneGraph, RenderMode::BlinnPhong);
}




