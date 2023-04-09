#include "kspch.h"
#include "application/application.h"
#include "application/entityPoint.h"
#include "renderer/Renderer.h"
#include "application/vendor/imGui/implot.h"
#include "application/vendor/imGui/imgui_internal.h"
#include "mesh/BasicMeshes/Box/Box.h"
#include "mesh/BasicMeshes/Sphere/Sphere.h"
#include "mesh/BasicMeshes/TorusKnot/TorusKnot.h"
#include "material/base/BaseMaterial.h"
#include "material/depth/DepthMaterial.h"
#include "material/wireframe/WireFrameMaterial.h"
#include "material/normalMap/NormalMaterial.h"
#include "material/environmentReflect/EnvironmentReflectMaterial.h"
#include "material/environmentRefract/EnvironmentRefractMaterial.h"
#include "material/pbr/PBRMaterial.h"
#include "material/pbr2/PBR2Material.h"

static std::unordered_map<std::string, float> refractIndex
{
	{ "air", 1.00f },
	{ "water", 1.33f },
	{ "ice", 1.309f },
	{ "glass", 1.52f },
	{ "damon", 2.42f },
};

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
static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
static ImGuiTreeNodeFlags leafTreeNodeFlags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
static int selection_mask = 0;
static int nodeIndex = 0;

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
		ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
		if (ImGui::TreeNodeEx(node->nodeName.c_str(), base_flags))
		{
			for (const auto& mesh : node->meshes)
			{
				nodeIndex++;
				const bool is_selected = (selection_mask & (1 << nodeIndex)) != 0;
				if (is_selected)
				{
					ImGui::TreeNodeEx(node->nodeName.c_str(), leafTreeNodeFlags | ImGuiTreeNodeFlags_Selected);
				}
				else {
					ImGui::TreeNodeEx(node->nodeName.c_str(), leafTreeNodeFlags);
				}
				
				if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				{
					selection_mask = 1 << nodeIndex;
				}					
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
		auto root = sceneLoader.loadModel("resource/models/Cerberus/Cerberus.obj", renderer);
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

		float mousedeltaX = 0.0f, mousedeltaY = 0.0f;
		if (payload.isHover && payload.io.MouseDown[1])
		{
			mousedeltaX = payload.io.MouseDelta.x;
			mousedeltaY = payload.io.MouseDelta.y;
		}
		camera->orbitControl(mousedeltaX, mousedeltaY, payload.io.MouseWheel, payload.viewport);
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
		//mesh auto rotate
		/*for (auto& root : sceneGraph.roots)
		{
			autoRotate(root, payload.ts);
		}*/
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
			nodeIndex = 0;
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
	}

	void autoRotate(std::shared_ptr<Node> node, float ts)
	{
		auto rotateMatrix = glm::rotate(glm::identity<glm::mat4x4>(), 0.003f, glm::vec3(0, 1, 0));
		for (auto& mesh : node->meshes)
		{
			glm::mat4x4 modelMatrix = mesh->getModelMatrix() * rotateMatrix;
			mesh->setModelMatrix(modelMatrix);
		}
		for (auto& n: node->children)
		{
			autoRotate(n, ts);
		}
	}

	SceneGraph& getSceneGraph() { return sceneGraph; };
	X_Renderer& getRenderer() { return renderer; };

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
	Layer* viewportLayer = new ViewportLayer();
	app->PushLayer(viewportLayer);
	app->setMenuCallback([]() {});
	/*app->setMenuCallback([app, viewportLayer]()
	{*/
		//if (ImGui::BeginMenu("Mesh"))
		//{
		//	if (ImGui::MenuItem("Box"))
		//	{
		//		//createBox
		//		auto node = std::make_shared<Node>();
		//		//MATERIAL TEST
		//		auto base = std::make_shared<BaseMaterial>(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.2, 0.3, 0.8), glm::vec3(1.0, 1.0, 1.0), 32.0f, viewportLayer->getRenderer());
		//		auto depth = std::make_shared<DepthMaterial>(viewportLayer->getRenderer());
		//		auto wireframe = std::make_shared<WireFrameMaterial>(viewportLayer->getRenderer());
		//		auto normal = std::make_shared<NormalMaterial>(viewportLayer->getRenderer());
		//		auto environmentReflectMaterial = std::make_shared<EnvironmentReflectMaterial>(viewportLayer->getRenderer());
		//		auto environmentRefractMaterial = std::make_shared<EnvironmentRefractMaterial>(viewportLayer->getRenderer(), refractIndex.find("glass")->second);

		//		node->meshes.push_back(std::make_shared<BoxMesh>("box", 10, 10, 20, environmentRefractMaterial));
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}

		//	if (ImGui::MenuItem("Sphere"))
		//	{
		//		// createSphere
		//			auto node = std::make_shared<Node>();
		//		//MATERIAL TEST
		//		auto base = std::make_shared<BaseMaterial>(glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.2, 0.3, 0.8), glm::vec3(1.0, 1.0, 1.0), 32.0f, viewportLayer->getRenderer());
		//		auto depth = std::make_shared<DepthMaterial>(viewportLayer->getRenderer());
		//		auto wireframe = std::make_shared<WireFrameMaterial>(viewportLayer->getRenderer());
		//		auto normal = std::make_shared<NormalMaterial>(viewportLayer->getRenderer());
		//		auto environmentReflectMaterial = std::make_shared<EnvironmentReflectMaterial>(viewportLayer->getRenderer());
		//		auto environmentRefractMaterial = std::make_shared<EnvironmentRefractMaterial>(viewportLayer->getRenderer(), refractIndex.find("damon")->second);

		//		auto sphere = std::make_shared<Sphere>("sphere");
		//		sphere->setMaterial(environmentRefractMaterial);
		//		node->meshes.push_back(sphere);
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}

		//	if (ImGui::MenuItem("PBRSphere"))
		//	{
		//		auto node = std::make_shared<Node>();
		//		int nrRows = 7;
		//		int nrColumns = 7;
		//		float spacing = 3.5f;
		//		glm::mat4x4 model = glm::identity<glm::mat4x4>();
		//		for (size_t i = 0; i < nrRows; i++)
		//		{
		//			float metallic = (float)i / (float)nrRows;
		//			for (size_t j = 0; j < nrColumns; j++)
		//			{
		//				float roughness = glm::clamp((float)j / (float)nrColumns, 0.05f, 2.0f);
		//				auto sphere = std::make_shared<Sphere>("sphere" + std::to_string(i) + std::to_string(j), 1.0f);
		//				auto pbrmaterial = std::make_shared<PbrMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), metallic, roughness, 1.0f, viewportLayer->getRenderer());
		//				sphere->setMaterial(pbrmaterial);
		//				model = glm::identity<glm::mat4x4>();
		//				model = glm::translate(model, glm::vec3(
		//					(j - (nrColumns / 2.0f)) * spacing + 2.0f,
		//					(i - (nrRows / 2.0f)) * spacing + 15.0f,
		//					0.0f
		//				));
		//				sphere->setModelMatrix(model);
		//				node->meshes.push_back(sphere);
		//			}
		//		}
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}

		//	if (ImGui::MenuItem("GlassSphere"))
		//	{
		//		auto node = std::make_shared<Node>();
		//		auto sphere = std::make_shared<Sphere>("metallicSphere", 4.5f);
		//		auto pbrmaterial = std::make_shared<PbrMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 1.0f, viewportLayer->getRenderer());
		//		sphere->setMaterial(pbrmaterial);
		//		glm::mat4x4 model = glm::identity<glm::mat4x4>();
		//		model = glm::translate(model, glm::vec3(
		//			0.0f,
		//			4.5f,
		//			0.0f
		//		));
		//		sphere->setModelMatrix(model);
		//		node->meshes.push_back(sphere);
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}

		//	if (ImGui::MenuItem("TorusKnot"))
		//	{
		//		auto node = std::make_shared<Node>();
		//		auto torusKnot = std::make_shared<TorusKnot>("TorusKnot", 18.0f, 8.0f, 150, 20);
		//		auto pbrmaterial = std::make_shared<PbrMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 1.0f, viewportLayer->getRenderer());
		//		torusKnot->setMaterial(pbrmaterial);
		//		glm::mat4x4 model = glm::identity<glm::mat4x4>();
		//		model = glm::translate(model, glm::vec3(
		//			0.0f,
		//			4.5f,
		//			0.0f
		//		));
		//		torusKnot->setModelMatrix(model);
		//		node->meshes.push_back(torusKnot);
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}

		//	if (ImGui::MenuItem("PBRSpheres"))
		//	{
		//		auto node = std::make_shared<Node>();
		//		std::vector<std::vector<std::shared_ptr<Texture>>> textures = {
		//			{
		//				std::make_shared<Texture>("resource/textures/pbr/Titanium-Scuffed-bl/albedo.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/Titanium-Scuffed-bl/metallic.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/Titanium-Scuffed-bl/roughness.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/Titanium-Scuffed-bl/normal.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/Titanium-Scuffed-bl/ao.png", TEXTURE_TYPE::SPECULAR)
		//			},
		//			{
		//				std::make_shared<Texture>("resource/textures/pbr/diamond-metal-siding-bl/albedo.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/diamond-metal-siding-bl/metallic.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/diamond-metal-siding-bl/roughness.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/diamond-metal-siding-bl/normal.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/diamond-metal-siding-bl/ao.png", TEXTURE_TYPE::SPECULAR)
		//			},
		//			{
		//				std::make_shared<Texture>("resource/textures/pbr/light-gold-bl/albedo.jpg", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/light-gold-bl/metallic.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/light-gold-bl/roughness.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/light-gold-bl/normal.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/light-gold-bl/ao.png", TEXTURE_TYPE::SPECULAR)
		//			},
		//			{
		//				std::make_shared<Texture>("resource/textures/pbr/fancy-brass-pattern1-bl/albedo.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/fancy-brass-pattern1-bl/metallic.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/fancy-brass-pattern1-bl/roughness.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/fancy-brass-pattern1-bl/normal.png", TEXTURE_TYPE::SPECULAR),
		//				std::make_shared<Texture>("resource/textures/pbr/fancy-brass-pattern1-bl/ao.png", TEXTURE_TYPE::SPECULAR)
		//			}
		//		};
		//		std::vector<std::shared_ptr<Pbr2Material>> mats;
		//		unsigned index = 0;
		//		for (auto& vecTex : textures)
		//		{
		//			auto mat = std::make_shared<Pbr2Material>(vecTex[0], vecTex[1], vecTex[2], vecTex[3], vecTex[4], viewportLayer->getRenderer());
		//			auto sphere = std::make_shared<Sphere>("metallicSphere", 3.0f);
		//			sphere->setMaterial(mat);
		//			glm::mat4x4 model = glm::identity<glm::mat4x4>();
		//			model = glm::translate(model, glm::vec3(
		//				-10.0f + index * 7.0f,
		//				3.0f,
		//				0.0f
		//			));
		//			sphere->setModelMatrix(model);
		//			node->meshes.push_back(sphere);
		//			index++;
		//		}							
		//		viewportLayer->getSceneGraph().roots.push_back(node);
		//	}
		//	ImGui::EndMenu();
		//}
	//});
	return app;
}