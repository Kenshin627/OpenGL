#include "Renderer.h"
#include "../program/BlinnPhong/BlinnPhong.h"
#include "../program/EnvironmentMapReflect/environmentMapReflect.h"
#include "../program/EnvironmentMapRefract/environmentMapRefract.h"
#include "../program/Depth/Depth.h"
#include "../program/WireFrame/WireFrame.h"
#include "../program/ShadowMap/ShadowMap.h"
#include "../program/BlinnPhongCastShadow/BlinnPhongCastShadow.h"
#include "../program/GridCastShadow/GridCastShadow.h"
#include "../program/Grid/GridShader.h"
#include "../program/VisualNormal/VisualNormal.h"
#include "../program/postProcess/EdgeDetection/EdgeDetection.h"
#include "../program/postProcess/GlitchRGBSplit/GlitchRGBSplit.h"
#include "../program/postProcess/GrayScale/GrayScale.h"
#include "../program/postProcess/Inversion/Inversion.h"
#include "../program/postProcess/NuClear/NuClear.h"

X_Renderer::X_Renderer():
	camera(std::make_shared<Camera>(glm::vec3(0, 17, 35), glm::vec3(0, 0, 0), glm::vec3{ 0,1,0 }, 800.0f / 600.0f, 0.1f, 500.0f, glm::radians(45.0f), 10.0f, 0.00006)), 
	m_FBO(std::make_shared<FrameBuffer>(1.0, 1.0)), 
	prevFBO(m_FBO),
	clearColor(glm::vec4(0.0, 0.0, 0.0, 1.0)), 
	mode(RenderMode::_BlinnPhong), 
	wireFrameColor(glm::vec3(0.5, 0.7, 0.2)),
	grid(std::make_shared<GridMesh>(200.0f)),
	quad(),
	skybox(
		{ "resource/textures/skyBox/right.jpg", "resource/textures/skyBox/left.jpg", 
		  "resource/textures/skyBox/top.jpg",   "resource/textures/skyBox/bottom.jpg", 
		  "resource/textures/skyBox/front.jpg", "resource/textures/skyBox/back.jpg" 
		}, 
		"shader/skyBox/vertex.glsl", 
		"shader/skyBox/fragment.glsl"
	),
	refractiveIndex(
	{
		{ "air", 1.00 },
		{ "water", 1.33 },
		{ "ice", 1.309 },
		{ "glass", 1.52 },
		{ "damon", 2.42 },
	}),
	outputTextureID(0),
	m_ShadowFBO(std::make_shared<ShadowFrameBuffer>(1.0, 1.0)),
	enableShadows(false),
	visiualNormal(false),
	postProcess(nullptr)
{
	lights.push_back(std::make_shared<DirectionLight>(glm::vec3(-1, -1, -1), glm::vec3(1.0f)));
	compileShaders();

	postProcess = shaderLib.find(ShaderType::GlitchRGBSplit)->second;

	#pragma region faceCulling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	#pragma endregion
}

X_Renderer::~X_Renderer() = default;

void X_Renderer::RenderShadow(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts)
{	
	clear();
	glViewport(0.0f, 0.0f, viewport.x, viewport.y);	

	std::shared_ptr<Shader> shadowShader = shaderLib.find(ShaderType::ShadowMap)->second;
	shadowShader->bind();	
	m_ShadowFBO->bind();
	shadowShader->setCommonUniforms();
	clear();
	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node);
	}
	grid->bind();
	shadowShader->setMeshUniforms(grid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
	grid->unbind();	
	m_ShadowFBO->unbind();
	shadowShader->unbind();
}

void X_Renderer::Render(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts)
{	
	clear();
	glViewport(0.0f, 0.0f, viewport.x, viewport.y);
	if (enableShadows)
	{
		RenderShadow(sceneGraph, viewport, ts);
	}
	
	m_FBO->bind();
	clear();

	#pragma region scene Graph render
	auto meshShaderType = enableShadows ? ShaderType::BlinnPhongCastShadow : ShaderType::BlinnPhong;
	std::shared_ptr<Shader> shader = shaderLib.find(meshShaderType)->second;
	shader->bind();	
	shader->setCommonUniforms();
	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node, shader);
	}
	shader->unbind();
	#pragma endregion

	#pragma region visual normal
	if (visiualNormal)
	{
		auto visualNormalShader = shaderLib.find(ShaderType::visualNormal)->second;
		visualNormalShader->bind();
		visualNormalShader->setCommonUniforms();
		for (const std::shared_ptr<Node>& node : sceneGraph.roots)
		{
			Recursivedraw(node, visualNormalShader);
		}
		visualNormalShader->unbind();
	}
	#pragma endregion

	#pragma region GRID 已完成
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	auto gridShaderType = enableShadows ? ShaderType::GridCastShadow : ShaderType::Grid;
	auto gridShader = shaderLib.find(gridShaderType)->second;
	gridShader->bind();
	grid->bind();	
	gridShader->setCommonUniforms();
	gridShader->setMeshUniforms(grid);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
	grid->unbind();
	gridShader->unbind();
	#pragma endregion

	#pragma region SkyBox
	skybox.bind();
	skybox.draw(*camera, 0);
	skybox.unbind();	
	#pragma endregion

	outputTextureID = m_FBO->GetTextureID();
	m_FBO->unbind();
		
	#pragma region postProcess
	if (postProcess != nullptr)
	{
		quad.bind();
		postProcess->bind();
		clear();
		postProcess->update(ts);
		postProcess->setCommonUniforms();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
		prevFBO = postProcess->getFBO();
		postProcess->unbind();
		outputTextureID = postProcess->getFBO()->GetTextureID();
		
		quad.unbind();
	}
	//prevFBO = m_FBO;
	#pragma endregion
}

void X_Renderer::Recursivedraw(const std::shared_ptr<Node>& node, std::shared_ptr<Shader> shader)
{
	if (!node->meshes.empty())
	{
		for (const std::shared_ptr<Mesh>& mesh : node->meshes)
		{			
			if (shader != nullptr)
			{
				shader->setMeshUniforms(mesh);
			}
			mesh->bind();
			glDrawElements(GL_TRIANGLES, mesh->indicesCount(), GL_UNSIGNED_INT, (const void*)0);
			mesh->unbind();
		}
	}
	if (!node->children.empty())
	{
		for (auto& node : node->children)
		{
			Recursivedraw(node, shader);
		}
	}
}

void X_Renderer::clear()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void X_Renderer::resizeFBO(unsigned width, unsigned height)
{
	m_FBO->resize(width, height);
	m_ShadowFBO->resize(width, height);
	for (auto& shader : shaderLib)
	{
		if (shader.second->getType() == ShaderCategory::PostProcessShader)
		{
			((PostProcess&)(*shader.second)).resetFBO(width, height);
		}
	}
}

void X_Renderer::compileShaders()
{
	shaderLib.insert({ ShaderType::WireFrame, std::make_shared<WireFrameShader>(std::vector<std::string>{ "shader/wireFrame/vertex.glsl", "shader/wireFrame/fragment.glsl" }, camera, wireFrameColor)});
	shaderLib.insert({ ShaderType::BlinnPhong, std::make_shared<BlinnPhongShader>(std::vector<std::string>{ "shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl" }, camera, lights[0])});
	//PBR暂未实现
	//shaderLib.insert({ ShaderType::PBR, std::make_shared<BlinnPhongShader>(std::vector<std::string>{ "shader/blinnPhongCastShadow/vertex.glsl", "shader/blinnPhongCastShadow/fragment.glsl" }, camera, lights[0]) });
	shaderLib.insert({ ShaderType::Depth, std::make_shared<DepthShader>(std::vector<std::string>{ "shader/depthRender/vertex.glsl", "shader/depthRender/fragment.glsl" }, camera) });
	shaderLib.insert({ ShaderType::Normal, std::make_shared<Shader>(std::vector<std::string>{ "shader/normal/vertex.glsl", "shader/normal/fragment.glsl" }) });
	shaderLib.insert({ ShaderType::Grid, std::make_shared<GridShader>(std::vector<std::string>{ "shader/grid/vertex.glsl", "shader/grid/fragment.glsl" }, camera, glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0, 0, 0), glm::vec4(1.0, 10, 0.33, .5)) });
	shaderLib.insert({ ShaderType::EnvironmentMapReflect, std::make_shared<EnvironmentMapReflectShader>(std::vector<std::string>{ "shader/environmentMapReflect/vertex.glsl", "shader/environmentMapReflect/fragment.glsl" }, camera, skybox.getTextureID())});
	shaderLib.insert({ ShaderType::EnvironmentMapRefract, std::make_shared<EnvironmentMapRefractShader>(std::vector<std::string>{ "shader/environmentMapRefract/vertex.glsl", "shader/environmentMapRefract/fragment.glsl" }, camera, skybox.getTextureID(), refractiveIndex.find("glass")->second)});
	shaderLib.insert({ ShaderType::visualNormal, std::make_shared<VisualNormalShader>(std::vector<std::string>{ "shader/visualNormal/vertex.glsl", "shader/visualNormal/fragment.glsl", "shader/visualNormal/geometry.glsl" }, camera, 0.7f, glm::vec3(0.2f, 0.5f, 0.6f)) });
	shaderLib.insert({ ShaderType::ShadowMap, std::make_shared<ShadowMapShader>(std::vector<std::string>{ "shader/shadowMap/vertex.glsl", "shader/shadowMap/fragment.glsl" }, lights[0])});
	shaderLib.insert({ ShaderType::BlinnPhongCastShadow, std::make_shared<BlinnPhongCastShadowShader>(std::vector<std::string>{ "shader/blinnPhongCastShadow/vertex.glsl", "shader/blinnPhongCastShadow/fragment.glsl" }, camera, lights[0], m_ShadowFBO) });
	shaderLib.insert({ ShaderType::GridCastShadow, std::make_shared<GridCastShadowShader>(std::vector<std::string>{ "shader/gridCastShadow/vertex.glsl", "shader/gridCastShadow/fragment.glsl" }, camera, lights[0], glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0, 0, 0), glm::vec4(1.0, 10, 0.33, .5), m_ShadowFBO) });

	//POSTPROCESS
	shaderLib.insert({ ShaderType::GrayScalize, std::make_shared<GrayScaleShader>(std::vector<std::string>{ "shader/grayScale/vertex.glsl", "shader/grayScale/fragment.glsl" }, prevFBO) });
	shaderLib.insert({ ShaderType::GlitchRGBSplit, std::make_shared<GlitchRGBSpliter>(std::vector<std::string>{ "shader/glitchRGBSplit/vertex.glsl", "shader/glitchRGBSplit/fragment.glsl" }, prevFBO, 10.0f, 0.2f, Direction::Horizontal) });
	shaderLib.insert({ ShaderType::Inversion, std::make_shared<InversionShader>(std::vector<std::string>{ "shader/inversion/vertex.glsl", "shader/inversion/fragment.glsl" }, prevFBO) });
	shaderLib.insert({ ShaderType::NuClear, std::make_shared<NuClearShader>(std::vector<std::string>{ "shader/nuclear/vertex.glsl", "shader/nuclear/fragment.glsl" }, prevFBO) });
	shaderLib.insert({ ShaderType::EdgeDetection, std::make_shared<EdgeDetectionShader>(std::vector<std::string>{ "shader/edgeDetection/vertex.glsl", "shader/edgeDetection/fragment.glsl" }, prevFBO) });
}

#pragma region lights

//void setLight(const Shader& program)
//{
//	//Lights
//	DirectionLight dl{ glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f) };
//	PointLight pl{ glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f), 50 };
//	SpotLight sl{ camera.getPosition(), camera.getForward(), glm::vec3(1.0f), 20.0f, 2.5f, 6.0f };
//
//	program.setVec3("directionLight.color", dl.getColor());
//	program.setVec3("directionLight.direction", dl.getDirection());
//
//	program.setVec3("pointLight.color", pl.getColor());
//	program.setVec3("pointLight.position", pl.getPosition());
//	program.setFloat("pointLight.kc", pl.getAttenuation().kc);
//	program.setFloat("pointLight.kl", pl.getAttenuation().kl);
//	program.setFloat("pointLight.kd", pl.getAttenuation().kd);
//
//	program.setVec3("spotLight.color", sl.getColor());
//	program.setVec3("spotLight.position", sl.getPosition());
//	program.setVec3("spotLight.direction", sl.getDirection());
//	program.setFloat("spotLight.kc", sl.getAttenuation().kc);
//	program.setFloat("spotLight.kl", sl.getAttenuation().kl);
//	program.setFloat("spotLight.kd", sl.getAttenuation().kd);
//	program.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(sl.getInnerCutOff())));
//	program.setFloat("spotLight.outterCutOff", glm::cos(glm::radians(sl.getOutterCutOff())));
//}

#pragma endregion
