#include "application/application.h"
#include "application/entityPoint.h"
#include "renderer/Renderer.h"

class ExampleLayer : public Kenshin::Layer
{
public:
	void onAttach() override
	{
		auto root = sceneLoader.loadModel("resource/models/nanosuit/nanosuit.obj");
		if (root)
		{
			sceneGraph.roots.push_back(root);
		}
	}
	void onUpdate(const glm::vec2& viewport, double timestep) override
	{
		
		renderer.Render(sceneGraph, RenderMode::BlinnPhong, viewport);

		ImGui::Image((void*)(intptr_t)(renderer.getFrameBufferTextureID()), ImVec2(viewport.x, viewport.y), ImVec2(0, 1), ImVec2(1, 0));
	};

	void onUIRender() override
	{
		ImGui::ShowDemoWindow();		
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