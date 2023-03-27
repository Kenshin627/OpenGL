#include "Renderer.h"
#include "postProcess/GrayScale/GrayScale.h"
#include "postProcess/GlitchRGBSplit/GlitchRGBSplit.h"
#include "postProcess/NuClear/NuClear.h"
#include "postProcess/EdgeDetection/EdgeDetection.h"

X_Renderer::X_Renderer():
	camera(std::make_shared<Camera>(glm::vec3(0, 17, 35), glm::vec3(0, 0, 0), glm::vec3{ 0,1,0 }, 800.0f / 600.0f, 0.1f, 500.0f, glm::radians(45.0f), 10.0f, 0.00006)), 
	m_FBO(std::make_shared<FrameBuffer>(1.0, 1.0)), 
	prevFBO(m_FBO),
	clearColor(glm::vec4(0.0, 0.0, 0.0, 1.0)), 
	mode(RenderMode::_BlinnPhong), 
	wireFrameColor(glm::vec3(0.5, 0.7, 0.2)),
	grid(200.0f),
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
	shadow_FBO(std::make_shared<ShadowFrameBuffer>(1.0, 1.0))
{
	lights.push_back(DirectionLight{ glm::vec3(0.3f, -0.7f, -1.0f), glm::vec3(1.0f) });
	compileShaders();
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
	glCullFace(GL_FRONT);
	clear();
	glViewport(0.0f, 0.0f, viewport.x, viewport.y);	

	std::shared_ptr<Shader> shadowShader = shaderLib.find(ShaderType::ShadowMap)->second;
	shadowShader->bind();	
	shadowShader->setMatrix44("model", glm::identity<glm::mat4x4>());
	shadowShader->setMatrix44("lightViewProjection", lights[0].getLightSpaceMatrix());

	shadow_FBO->bind();
	clear();

	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node);
	}

	grid.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
	grid.unbind();
	shadow_FBO->unbind();
	glCullFace(GL_BACK);
}

void X_Renderer::Render(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts)
{
	if (mode == RenderMode::_WireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	RenderShadow(sceneGraph, viewport, ts);

	clear();
	glViewport(0.0f, 0.0f, viewport.x, viewport.y);

	#pragma region scene Graph render
	std::shared_ptr<Shader> shader = shaderLib.find((ShaderType)mode)->second;
	shader->bind();
	/*setLight(program);*/
	shader->setVec3("directionLight.color", lights[0].getColor());
	shader->setVec3("directionLight.direction", lights[0].getDirection());

	glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>();
	glm::mat3x3 modelInverseTranspose = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
	shader->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	shader->setVec3("cameraPosition", camera->getPosition());
	shader->setMatrix44("model", modelMatrix);
	shader->setMatrix33("modelInverseTranspose", modelInverseTranspose);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTextureID());
	shader->setInt("skyBox", 0);
	shader->setFloat("refractiveIndex", refractiveIndex.find("damon")->second);

	//linear Depth
	shader->setFloat("near", camera->getNear());
	shader->setFloat("far", camera->getFar());

	//wireFrame
	shader->setVec3("wireFrameColor", wireFrameColor);

	//shadow
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, shadow_FBO->GetTextureID());
	shader->setInt("shadowMap", 6);
	shader->setMatrix44("lightPosSpace", lights[0].getLightSpaceMatrix());

	m_FBO->bind();
	clear();
	for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	{
		Recursivedraw(node, shader);
	}
	shader->unbind();
	#pragma endregion

	#pragma region visual normal
	//auto visualNormalProgram = shaders.find(RenderMode::visualNormal);
	//visualNormalProgram->second->bind();
	////visual Normal
	//visualNormalProgram->second->setMatrix44("modelView", camera->viewMatrix() * modelMatrix);
	//visualNormalProgram->second->setMatrix33("inverseModelView", glm::mat3x3(glm::transpose(glm::inverse(camera->viewMatrix() * modelMatrix))));
	//visualNormalProgram->second->setMatrix44("projection", camera->projMatrix());
	//visualNormalProgram->second->setFloat("magnitude", 0.5);
	//visualNormalProgram->second->setVec3("lineColor", glm::vec3(0.3, 0.6, 0.8));

	//for (const std::shared_ptr<Node>& node : sceneGraph.roots)
	//{
	//	Recursivedraw(node, *visualNormalProgram->second);
	//}
	//visualNormalProgram->second->unbind();
	#pragma endregion

	#pragma region GRID ÒÑÍê³É
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Shader& gridShader = *shaderLib.find(ShaderType::GridCastShadow)->second;
	gridShader.bind();
	gridShader.setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
	gridShader.setVec2("viewport", viewport);
	gridShader.setVec3("mainColor", glm::vec3(0.0, 0.0, 0.0));
	gridShader.setVec3("lineColor", glm::vec3(.0, 0.5, 0.5));
	//gridRatio, majorUnitFrequency, minorUnitVisibility, opacity
	gridShader.setVec4("gridControl", glm::vec4(1.0, 10, 0.33, .5));
	gridShader.setVec3("gridOffset", glm::vec3(0, 0, 0));

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, shadow_FBO->GetTextureID());
	gridShader.setInt("shadowMap", 6);
	gridShader.setMatrix44("lightPosSpace", lights[0].getLightSpaceMatrix());
	grid.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
	grid.unbind();
	#pragma endregion

	#pragma region SkyBox
	skybox.bind();
	skybox.draw(*camera, 0);
	skybox.unbind();

	outputTextureID = m_FBO->GetTextureID();
	m_FBO->unbind();
	#pragma endregion
		
	#pragma region postProcess
	quad.bind();
	//if (mode == RenderMode::BlinnPhong)
	//{
	//	for (auto& postProcess : postProcesses)
	//	{
	//		postProcess.second->update(ts);
	//		postProcess.second->bind();
	//		clear();
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, prevFBO->GetTextureID());
	//		postProcess.second->draw(0);
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//		prevFBO = postProcess.second->getFBO();
	//		postProcess.second->unbind();
	//	}
	//	outputTextureID = prevFBO->GetTextureID();
	//	prevFBO = m_FBO;
	//}
	quad.unbind();
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
				auto mat = mesh->getMaterial();
				if (!mat->ambientTextures.empty())
				{
					mat->ambientTextures[0]->bind(0);
					shader->setInt("material.ambientTexture", 0);
				}
				if (!mat->diffuseTextures.empty())
				{
					mat->diffuseTextures[0]->bind(0);
					shader->setInt("material.diffuseTexture", 0);
				}
				if (!mat->specularTextures.empty())
				{
					mat->specularTextures[0]->bind(2);
					shader->setInt("material.specularTexture", 2);
				}
				shader->setFloat("material.shininess", mat->shininess ? mat->shininess : 64.0);
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
	shadow_FBO->resize(width, height);
	/*for (auto& postProcess : postProcesses)
	{
		postProcess.second->resetFBO(width, height);
	}*/
}

void X_Renderer::compileShaders()
{
	shaderLib.insert({ ShaderType::WireFrame, std::make_shared<Shader>("shader/wireFrame/vertex.glsl", "shader/wireFrame/fragment.glsl") });
	shaderLib.insert({ ShaderType::BlinnPhong, std::make_shared<Shader>("shader/blinnPhongCastShadow/vertex.glsl", "shader/blinnPhongCastShadow/fragment.glsl") });	
	shaderLib.insert({ ShaderType::PBR, std::make_shared<Shader>("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl") });
	shaderLib.insert({ ShaderType::Depth, std::make_shared<Shader>("shader/depthRender/vertex.glsl", "shader/depthRender/fragment.glsl") });
	shaderLib.insert({ ShaderType::Normal, std::make_shared<Shader>("shader/normal/vertex.glsl", "shader/normal/fragment.glsl") });
	shaderLib.insert({ ShaderType::Grid, std::make_shared<Shader>("shader/grid/vertex.glsl", "shader/grid/fragment.glsl") });
	shaderLib.insert({ ShaderType::EnvironmentMapReflect, std::make_shared<Shader>("shader/environmentMapReflect/vertex.glsl", "shader/environmentMapReflect/fragment.glsl") });
	shaderLib.insert({ ShaderType::EnvironmentMapRefract, std::make_shared<Shader>("shader/environmentMapRefract/vertex.glsl", "shader/environmentMapRefract/fragment.glsl") });
	shaderLib.insert({ ShaderType::visualNormal, std::make_shared<Shader>("shader/visualNormal/vertex.glsl", "shader/visualNormal/fragment.glsl", "shader/visualNormal/geometry.glsl") });
	shaderLib.insert({ ShaderType::ShadowMap, std::make_shared<Shader>("shader/shadowMap/vertex.glsl", "shader/shadowMap/fragment.glsl") });
	shaderLib.insert({ ShaderType::BlinnPhongCastShadow, std::make_shared<Shader>("shader/blinnPhongCastShadow/vertex.glsl", "shader/blinnPhongCastShadow/fragment.glsl") });
	shaderLib.insert({ ShaderType::GridCastShadow, std::make_shared<Shader>("shader/gridCastShadow/vertex.glsl", "shader/gridCastShadow/fragment.glsl") });

	shaderLib.insert({ ShaderType::GrayScalize, std::make_shared<Shader>("shader/grayScale/vertex.glsl", "shader/grayScale/fragment.glsl") });
	shaderLib.insert({ ShaderType::GlitchRGBSplit, std::make_shared<Shader>("shader/glitchRGBSplit/vertex.glsl", "shader/glitchRGBSplit/fragment.glsl") });
	shaderLib.insert({ ShaderType::Inversion, std::make_shared<Shader>("shader/inversion/vertex.glsl", "shader/inversion/fragment.glsl") });
	shaderLib.insert({ ShaderType::NuClear, std::make_shared<Shader>("shader/nuclear/vertex.glsl", "shader/nuclear/fragment.glsl") });
	shaderLib.insert({ ShaderType::EdgeDetection, std::make_shared<Shader>("shader/edgeDetection/vertex.glsl", "shader/edgeDetection/fragment.glsl") });
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
