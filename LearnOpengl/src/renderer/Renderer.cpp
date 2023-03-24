#include "Renderer.h"
#include "postProcess/GrayScale/GrayScale.h"
#include "postProcess/GlitchRGBSplit/GlitchRGBSplit.h"

X_Renderer::X_Renderer():
	camera(std::make_shared<Camera>(glm::vec3(0, 8, 23), glm::vec3(0, 0, -1), glm::vec3{ 0,1,0 }, 800.0f / 600.0f, 0.1f, 100.0f, glm::radians(45.0f), 10.0f, 0.06)), 
	m_FBO(std::make_shared<FrameBuffer>(1.0, 1.0)), 
	prevFBO(m_FBO),
	clearColor(glm::vec4(0.2, 0.2, 0.2, 1.0)), 
	mode(RenderMode::BlinnPhong), 
	wireFrameColor(glm::vec3(0.5, 0.7, 0.2)),
	grid("grid"),
	quad()
{
	lights.push_back(DirectionLight{ glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f) });
	compileShaders();
	compilePostProcess();
}

X_Renderer::~X_Renderer() {}

void X_Renderer::Render(const SceneGraph& sceneGraph, const glm::vec2& viewport, float ts)
{
	glEnable(GL_DEPTH_TEST);
	if (mode == RenderMode::wireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else {	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	clear();
	glViewport(0.0f, 0.0f, viewport.x, viewport.y);
	auto programIter = shaders.find(mode);
	if (programIter != shaders.cend())
	{
		programIter->second->bind();
		/*setLight(program);*/
		programIter->second->setVec3("directionLight.color", lights[0].getColor());
		programIter->second->setVec3("directionLight.direction", lights[0].getDirection());

		#pragma region DRAW CALL

		glm::mat4x4 modelMatrix = glm::identity<glm::mat4x4>();
		modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
		glm::mat3x3 modelInverseTranspose = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
		programIter->second->setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
		programIter->second->setVec3("cameraPosition", camera->getPosition());
		programIter->second->setMatrix44("model", modelMatrix);
		programIter->second->setMatrix33("modelInverseTranspose", modelInverseTranspose);

		//linear Depth
		programIter->second->setFloat("near", camera->getNear());
		programIter->second->setFloat("far", camera->getFar());

		//wireFrame
		programIter->second->setVec3("wireFrameColor", wireFrameColor);

		m_FBO->bind();
		clear();
		for (const std::shared_ptr<Node>& node : sceneGraph.roots)
		{
			Recursivedraw(node, *programIter->second);
		}
		programIter->second->unbind();

		#pragma region GRID 未完成
		//render plane
		/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		auto gridShader = *shaders.find(RenderMode::grid)->second;
		gridShader.bind();
		gridShader.setMatrix44("modelViewProjection", camera->projMatrix() * camera->viewMatrix() * modelMatrix);
		gridShader.setVec2("viewport", viewport.x, viewport.y);
		grid.bind();
		glDrawElements(GL_TRIANGLES, grid.indicesCount(), GL_UNSIGNED_INT, (const void*)0);
		grid.unbind();*/
		#pragma endregion

		m_FBO->unbind();

		#pragma region postProcess
		quad.bind();
		//glDisable(GL_DEPTH_TEST);
		for (auto& postProcess : postProcesses)
		{
			postProcess.second->update(ts);
			postProcess.second->bind();
			clear();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, prevFBO->GetTextureID());
			postProcess.second->draw(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			prevFBO = postProcess.second->getFBO();		
			postProcess.second->unbind();
		}
		outputTextureID = prevFBO->GetTextureID();
		prevFBO = m_FBO;
		quad.unbind();
		#pragma endregion
		#pragma endregion
	}
	else {
		std::cout << "not Found asective shaders!" << std::endl;
	}
}

void X_Renderer::Recursivedraw(const std::shared_ptr<Node>& node, const Shader& p)
{
	if (!node->meshes.empty())
	{
		for (const std::shared_ptr<Mesh>& mesh : node->meshes)
		{
			mesh->bind();
			auto mat = mesh->getMaterial();
			if (!mat->ambientTextures.empty())
			{
				mat->ambientTextures[0]->bind(0);
				p.setInt("material.ambientTexture", 0);
			}
			if (!mat->diffuseTextures.empty())
			{
				mat->diffuseTextures[0]->bind(0);
				p.setInt("material.diffuseTexture", 0);
			}
			if (!mat->specularTextures.empty())
			{
				mat->specularTextures[0]->bind(2);
				p.setInt("material.specularTexture", 2);
			}
			p.setFloat("material.shininess", mat->shininess ? mat->shininess : 32.0);
			glDrawElements(GL_TRIANGLES, mesh->indicesCount(), GL_UNSIGNED_INT, (const void*)0);
			mesh->unbind();
		}
	}
	if (!node->children.empty())
	{
		for (auto& node : node->children)
		{
			Recursivedraw(node, p);
		}
	}
}

void X_Renderer::clear()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void X_Renderer::buildFBO(const glm::vec2& viewport)
{
	if (m_FBO)
	{
		m_FBO.reset();
	}
	m_FBO = std::make_unique<FrameBuffer>(viewport.x, viewport.y);
}

void X_Renderer::resizeFBO(unsigned width, unsigned height)
{
	m_FBO->resize(width, height);
	for (auto& postProcess : postProcesses)
	{
		postProcess.second->resetFBO(width, height);
	}
}

void X_Renderer::compileShaders()
{
	shaders.insert({ RenderMode::wireFrame, std::make_shared<Shader>("shader/wireFrame/vertex.glsl", "shader/wireFrame/fragment.glsl") });
	shaders.insert({ RenderMode::BlinnPhong, std::make_shared<Shader>("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl") });
	//PBR暂时未实现
	shaders.insert({ RenderMode::PBR, std::make_shared<Shader>("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl") });
	shaders.insert({ RenderMode::Depth, std::make_shared<Shader>("shader/depthRender/vertex.glsl", "shader/depthRender/fragment.glsl") });
	shaders.insert({ RenderMode::Normal, std::make_shared<Shader>("shader/normal/vertex.glsl", "shader/normal/fragment.glsl") });
	shaders.insert({ RenderMode::grid, std::make_shared<Shader>("shader/grid/vertex.glsl", "shader/grid/fragment.glsl") });
}

void X_Renderer::compilePostProcess()
{
	//compile grayScale shader
	//postProcesses.insert(std::make_pair<PostProcessMode, std::shared_ptr<PostProcess>>(PostProcessMode::GrayScalize, std::make_shared<GrayScale>("grayScale", "shader/grayScale/vertex.glsl", "shader/grayScale/fragment.glsl", PostProcessMode::GrayScalize)));
	postProcesses.insert(std::make_pair<PostProcessMode, std::shared_ptr<PostProcess>>(PostProcessMode::GlitchRGBSplit, std::make_shared<GlitchRGBSpliter>("grayScale", "shader/GlitchRGBSplit/vertex.glsl", "shader/GlitchRGBSplit/fragment.glsl", PostProcessMode::GlitchRGBSplit, 10.0f, 0.5f, Direction::Horizontal)));
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
