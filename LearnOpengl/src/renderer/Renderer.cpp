#include <glad/gl.h>
#include "Renderer.h"
#include "../mesh/BasicMeshes/Grid/Grid.h"
#include "../material/grid/GridMaterial.h"
#include "../material/depth/DepthMaterial.h"

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
	outputTextureID(0),
	m_ShadowFBO(std::make_shared<ShadowFrameBuffer>(1.0, 1.0)),
	enableShadows(false),
	visiualNormal(false),
	postProcess(nullptr)
{
	directionLights.push_back(std::make_shared<DirectionLight>(glm::vec3(-1, -1, -1), glm::vec3(1.0f)));
	CompileShaders();

	pointLights.push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 32));
	pointLights.push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 32));
	pointLights.push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 32));
	pointLights.push_back(std::make_shared<PointLight>(glm::vec3(10.0f, -10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f), 32));

	grid->setMaterial(std::make_shared<GridMaterial>(glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0, 0, 0), glm::vec4(1.0, 10, 0.33, .5), *this));
	
	//postProcess = shaderLib.find(ShaderType::GlitchRGBSplit)->second;

	#pragma region faceCulling
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	#pragma endregion
}

X_Renderer::~X_Renderer() = default;

void X_Renderer::RenderShadow(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts)
{	
	clear();
	glViewport(0, 0, viewport.x, viewport.y);	

	std::shared_ptr<Shader> shadowShader = shaderLib.find(ShaderType::ShadowMap)->second;
	shadowShader->bind();	
	m_ShadowFBO->bind();
	//shadowShader->setCommonUniforms();
	clear();
	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node);
	}
	grid->draw();
	m_ShadowFBO->unbind();
	shadowShader->unbind();
}

void X_Renderer::Render(const SceneGraph& sceneGraph, const glm::ivec2& viewport, float ts)
{	
	clear();
	glViewport(0, 0, viewport.x, viewport.y);
	if (enableShadows)
	{
		RenderShadow(sceneGraph, viewport, ts);
	}
	
	m_FBO->bind();
	clear();

	#pragma region scene Graph render
	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node);
	}
	#pragma endregion

	#pragma region visual normal
	if (visiualNormal)
	{
		auto visualNormalShader = shaderLib.find(ShaderType::visualNormal)->second;
		visualNormalShader->bind();
		//visualNormalShader->setCommonUniforms();
		for (const std::shared_ptr<Node>& node : sceneGraph.roots)
		{
			Recursivedraw(node);
		}
		visualNormalShader->unbind();
	}
	#pragma endregion

	#pragma region GRID ÒÑÍê³É
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	grid->draw();
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
		//postProcess->setCommonUniforms();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
		prevFBO = postProcess->getFBO();
		postProcess->unbind();
		outputTextureID = postProcess->getFBO()->GetTextureID();
		
		quad.unbind();
	}
	//prevFBO = m_FBO;
	#pragma endregion
}

void X_Renderer::Recursivedraw(const std::shared_ptr<Node>& node)
{
	if (!node->meshes.empty())
	{
		for (const std::shared_ptr<Mesh>& mesh : node->meshes)
		{			
			mesh->draw();
		}
	}
	if (!node->children.empty())
	{
		for (auto& node : node->children)
		{
			Recursivedraw(node);
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
			//((PostProcess&)(*shader.second)).resetFBO(width, height);
		}
	}
}

void X_Renderer::CompileShaders()
{
	shaderLib.insert({ ShaderType::WireFrame, std::make_shared<Shader>(std::vector<std::string>{ "shader/wireFrame/vertex.glsl", "shader/wireFrame/fragment.glsl" })});
	shaderLib.insert({ ShaderType::BlinnPhong, std::make_shared<Shader>(std::vector<std::string>{ "shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl"})});
	shaderLib.insert({ ShaderType::Depth, std::make_shared<Shader>(std::vector<std::string>{ "shader/depthRender/vertex.glsl", "shader/depthRender/fragment.glsl" })});
	shaderLib.insert({ ShaderType::Normal, std::make_shared<Shader>(std::vector<std::string>{ "shader/normal/vertex.glsl", "shader/normal/fragment.glsl"})});
	shaderLib.insert({ ShaderType::Grid, std::make_shared<Shader>(std::vector<std::string>{ "shader/grid/vertex.glsl", "shader/grid/fragment.glsl"})});
	shaderLib.insert({ ShaderType::EnvironmentMapReflect, std::make_shared<Shader>(std::vector<std::string>{ "shader/environmentMapReflect/vertex.glsl", "shader/environmentMapReflect/fragment.glsl" })});
	shaderLib.insert({ ShaderType::EnvironmentMapRefract, std::make_shared<Shader>(std::vector<std::string>{ "shader/environmentMapRefract/vertex.glsl", "shader/environmentMapRefract/fragment.glsl" })});
	shaderLib.insert({ ShaderType::visualNormal, std::make_shared<Shader>(std::vector<std::string>{ "shader/visualNormal/vertex.glsl", "shader/visualNormal/fragment.glsl", "shader/visualNormal/geometry.glsl" })});
	shaderLib.insert({ ShaderType::ShadowMap, std::make_shared<Shader>(std::vector<std::string>{ "shader/shadowMap/vertex.glsl", "shader/shadowMap/fragment.glsl" })});
	shaderLib.insert({ ShaderType::BlinnPhongCastShadow, std::make_shared<Shader>(std::vector<std::string>{ "shader/blinnPhongCastShadow/vertex.glsl", "shader/blinnPhongCastShadow/fragment.glsl" })});
	shaderLib.insert({ ShaderType::GridCastShadow, std::make_shared<Shader>(std::vector<std::string>{ "shader/gridCastShadow/vertex.glsl", "shader/gridCastShadow/fragment.glsl" })});
	shaderLib.insert({ ShaderType::PBR, std::make_shared<Shader>(std::vector<std::string>{ "shader/pbr/vertex.glsl", "shader/pbr/fragment.glsl"}) });

	//POSTPROCESS
	shaderLib.insert({ ShaderType::GrayScalize, std::make_shared<Shader>(std::vector<std::string>{ "shader/grayScale/vertex.glsl", "shader/grayScale/fragment.glsl" })});
	shaderLib.insert({ ShaderType::GlitchRGBSplit, std::make_shared<Shader>(std::vector<std::string>{ "shader/glitchRGBSplit/vertex.glsl", "shader/glitchRGBSplit/fragment.glsl" })});
	shaderLib.insert({ ShaderType::Inversion, std::make_shared<Shader>(std::vector<std::string>{ "shader/inversion/vertex.glsl", "shader/inversion/fragment.glsl" })});
	shaderLib.insert({ ShaderType::NuClear, std::make_shared<Shader>(std::vector<std::string>{ "shader/nuclear/vertex.glsl", "shader/nuclear/fragment.glsl" })});
	shaderLib.insert({ ShaderType::EdgeDetection, std::make_shared<Shader>(std::vector<std::string>{ "shader/edgeDetection/vertex.glsl", "shader/edgeDetection/fragment.glsl" })});
	shaderLib.insert({ ShaderType::Base, std::make_shared<Shader>(std::vector<std::string>{ "shader/base/vertex.glsl", "shader/base/fragment.glsl" })});
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
