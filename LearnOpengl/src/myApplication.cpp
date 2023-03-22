#include "application/application.h"
#include "application/entityPoint.h"
#include "renderer/Renderer.h"
#include "application/vendor/imGui/implot.h"
#include "application/vendor/imGui/imgui_internal.h"


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
		auto root = sceneLoader.loadModel("resource/models/nanosuit/nanosuit.obj");
		if (root)
		{
			sceneGraph.roots.push_back(root);
		}
	}
	void onUpdate(const glm::vec2& viewport, double timestep) override
	{
		renderer.getCamera()->setRatio(viewport.x / viewport.y);
		renderer.Render(sceneGraph, viewport);
		ImGui::Image((void*)(intptr_t)(renderer.getFrameBufferTextureID()), ImVec2(viewport.x, viewport.y), ImVec2(0, 1), ImVec2(1, 0));
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
		const char* items[] = { "wireFrame", "BlinnPhong", "PBR", "Depth" };
		static int item_current = 0;
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
	app->pushLayer<ViewportLayer>();
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