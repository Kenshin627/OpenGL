#include "application/application.h"
#include "application/entityPoint.h"
#include "renderer/Renderer.h"
#include "application/vendor/imGui/implot.h"
#include "application/vendor/imGui/imgui_internal.h"
#include "mesh/BasicMeshes/Box/Box.h"
#include "material/base/BaseMaterial.h"
#include "material/depth/DepthMaterial.h"

struct ScrollingBuffer {
	int MaxSize;
	int Offset;
	ImVector<ImVec2> Data;
	ScrollingBuffer(int max_size = 2000) {
		MaxSize = max_size;
		Offset = 0;
		Data.reserve(MaxSize);
	}
	void AddPoint(float x, float y) {
		if (Data.size() < MaxSize)
			Data.push_back(ImVec2(x, y));
		else {
			Data[Offset] = ImVec2(x, y);
			Offset = (Offset + 1) % MaxSize;
		}
	}
	void Erase() {
		if (Data.size() > 0) {
			Data.shrink(0);
			Offset = 0;
		}
	}
};

class ViewportLayer : public Kenshin::Layer
{
public:
	enum MeshType
	{
		Box,
		Sphere
	};
	void makeTree(const std::shared_ptr<Node>& node) const
	{
		if (ImGui::TreeNode(node->nodeName.c_str()))
		{
			for (const auto& mesh : node->meshes)
			{
				ImGui::Text(mesh->getName().c_str());				
			}
			for (const auto& child : node->children)
			{
				makeTree(child);
			}
			ImGui::TreePop();
		}
	}

	void onAttach() override
	{
		auto root = sceneLoader.loadModel("resource/models/nanosuit/nanosuit.obj", renderer);
		if (root)
		{
			sceneGraph.roots.push_back(root);
		}
	}

	void onUpdate(const Kenshin::updatePayload& payload) override
	{
		std::shared_ptr<Camera> camera = renderer.getCamera();
		std::shared_ptr<DirectionLight> light = renderer.getLights()[0];
		auto pos = light->getPostion();
		auto deltaX = sin((payload.ts)) * 0.05 * 2;
		auto deltaY = cos((payload.ts)) * 0.05 * 2;
		auto deltaZ = cos((payload.ts)) * 0.05 * 2;
		auto dir = glm::vec3(0, 0, 0) - glm::vec3(deltaX + pos.x, deltaY + pos.y, deltaZ + pos.z);
		light->setDirection(dir);
		if (m_viewportSize.x != payload.viewport.x || m_viewportSize.y != payload.viewport.y)
		{
			m_viewportSize.x = payload.viewport.x;
			m_viewportSize.y = payload.viewport.y;
			camera->setRatio(payload.viewport.x / payload.viewport.y);
			renderer.resizeFBO(m_viewportSize.x, m_viewportSize.y);
		}

		if (payload.isHover && payload.io.MouseDown[1])
		{
			camera->pitchYaw(payload.io.MouseDelta.x, payload.io.MouseDelta.y);
		}

		if (payload.isHover && payload.isfocus)
		{
			for (auto& key : camera->getKeybordConfig())
			{
				if (ImGui::IsKeyDown(key.first))
				{
					camera->move(key.second, payload.deltaTime);
					break;
				}
			}
		}

		renderer.Render(sceneGraph, m_viewportSize, payload.ts);
		ImGui::Image((void*)(intptr_t)(renderer.getFrameBufferTextureID()), ImVec2((float)m_viewportSize.x, (float)m_viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
	};

	void onUIRender(float fps, float deltaTime) override
	{
		#pragma region examples
		ImGui::ShowDemoWindow();
		#pragma endregion

		#pragma region Scene Graph
		ImGui::Begin("SceneGraph");
		if (ImGui::TreeNode("Scene TreeNode"))
		{
			for (auto& root : sceneGraph.roots)
			{
				makeTree(root);
			}
			ImGui::TreePop();
		}
		ImGui::End();
		#pragma endregion

		#pragma region FPS		
		ImGui::Begin("statistics");
		ImPlot::CreateContext();
		static float t = 0;
		t += ImGui::GetIO().DeltaTime;
		static ScrollingBuffer dataFps;
		static ScrollingBuffer dataDeltaTime;
		dataFps.AddPoint(t, fps);
		dataDeltaTime.AddPoint(t, deltaTime);

		static float history = 5.0f;
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels | ImPlotFlags_NoTitle;

		if (ImPlot::BeginPlot("FPS", ImVec2(-1, 90), ImPlotFlags_NoTitle)) {
			ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
			ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 200);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotLine("FPS", &dataFps.Data[0].x, &dataFps.Data[0].y, dataFps.Data.size(), 0, dataFps.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}
		if (ImPlot::BeginPlot("deltaTime", ImVec2(-1, 90), ImPlotFlags_NoTitle))
		{
			ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
			ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 0.0333F);
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotLine("Deltatime", &dataDeltaTime.Data[0].x, &dataDeltaTime.Data[0].y, dataDeltaTime.Data.size(), 0, dataDeltaTime.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}
		//ImPlot::ShowDemoWindow();
		ImGui::End();
		#pragma endregion

		#pragma region RenderMode
		ImGui::Begin("Render Mode");
		const char* items[] = { "wireFrame", "BlinnPhong", "PBR", "Depth", "Normal", "grid", "EnvironmentMapReflect", "EnvironmentMapRefract" };
		static int item_current = 1;
		if (ImGui::Combo(" ", &item_current, items, IM_ARRAYSIZE(items)))
		{
			renderer.setRenderMode((RenderMode)item_current);
		}
		if (item_current == 0)
		{
			ImGui::ColorPicker3("wireFrameColor", &renderer.getWireFrameColor().x);
		}

		ImGui::End();
		#pragma endregion

		#pragma region light
		ImGui::Begin("light Direction");
		//IMGUI_DEMO_MARKER("Widgets/Color/ColorEdit (float display)");

		glm::vec3 lightDirection = renderer.getLights()[0]->getDirection();
		ImGui::ColorEdit3("direction", /*(float*)&color */ (float*) &lightDirection, ImGuiColorEditFlags_Float);
		renderer.getLights()[0]->setDirection(lightDirection);
		ImGui::ColorEdit3("color", /*(float*)&color */ (float*)&renderer.getLights()[0]->getColor(), ImGuiColorEditFlags_Float);
		ImGui::End();
		#pragma endregion

	}

	void createMesh(MeshType type)
	{
		
	}

	SceneGraph& getSceneGraph() { return sceneGraph; };
	const X_Renderer& getRenderer() const { return renderer; };

private:
	X_Renderer renderer;
	SceneGraph sceneGraph;
	SceneLoader sceneLoader;
	glm::vec2 m_viewportSize = { 800, 600 };
};
								
Kenshin::Application* Kenshin::createApplication(int argc, char** argv)
{
	ApplicationSpecification spec { "X_Renderer", 1600, 900 };
	Application* app = new Application(spec);
	std::shared_ptr<ViewportLayer> viewportLayer = std::make_shared<ViewportLayer>();
	app->pushLayer(viewportLayer);
	app->setMenuCallback([app, viewportLayer]()
	{
		if (ImGui::BeginMenu("Mesh"))
		{
			if (ImGui::MenuItem("createBox"))
			{
				//createBox
				auto node = std::make_shared<Node>();
				//std::make_shared<BaseMaterial>(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.2, 0.3, 0.8), glm::vec3(1.0, 1.0, 1.0), 32.0f, viewportLayer->getRenderer())

				node->meshes.push_back(std::make_shared<BoxMesh>("box", 10, 10, 50, std::make_shared<DepthMaterial>(viewportLayer->getRenderer())));
				viewportLayer->getSceneGraph().roots.push_back(node);
			}
			ImGui::EndMenu();
		}
	});
	return app;
}