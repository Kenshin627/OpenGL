#include "application/application.h"
#include "application/entityPoint.h"
#include "renderer/Renderer.h"
//#include "application/vendor/imGui/imgui.h"

class ExampleLayer : public Kenshin::Layer
{
public:
	void onUpdate(double timestep) override
	{
		renderer = X_Renderer();
		sceneGraph = SceneGraph();
		auto root = sceneLoader.loadModel("resource/models/nanosuit/nanosuit.obj");
		if (root)
		{
			sceneGraph.roots.push_back(root);
		}
	};

	void onUIRender() override
	{
		//ImGui::ShowDemoWindow();		
		renderer.Render(sceneGraph, RenderMode::BlinnPhong);
	}
private:
	X_Renderer renderer;
	SceneGraph sceneGraph;
	SceneLoader sceneLoader;
};
								
Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	ApplicationSpecification spec { "X_Renderer", 1600, 900 };
	Application* app = new Application(spec);
	app->pushLayer<ExampleLayer>();
	app->setMenuCallback([app]()
	{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->close();
				}
				ImGui::EndMenu();
			}
	});
	return app;
}