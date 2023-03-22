#include "Renderer.h"

X_Renderer::X_Renderer() :camera(std::make_shared<Camera>(glm::vec3(0, 8, 23), glm::vec3(0, 0, -1), glm::vec3{ 0,1,0 }, 800.0f / 600.0f, 0.1f, 100.0f, glm::radians(45.0f), 10.0f, 0.06)), m_FBO(nullptr), clearColor(glm::vec4(0.2, 0.2, 0.2, 1.0)), mode(RenderMode::wireFrame), wireFrameColor(glm::vec3(0.5, 0.7, 0.2))
{
	lights.push_back(DirectionLight{ glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f) });
	compileShaders();
}

X_Renderer::~X_Renderer() {}

void X_Renderer::Render(const SceneGraph& sceneGraph, const glm::vec2& viewport)
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

		/**-----DRAW CALL-------------------------------**/
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

		buildFBO(viewport);
		m_FBO->bind();
		clear();
		for (const std::shared_ptr<Node>& node : sceneGraph.roots)
		{
			Recursivedraw(node, *programIter->second);
		}
		m_FBO->unbind();
		/**-----DRAW CALL END---------------------------- */
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
			std::cout << mesh->indicesCount() << std::endl;
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

void X_Renderer::compileShaders()
{
	shaders.insert({ RenderMode::wireFrame, std::make_shared<Shader>("shader/wireFrame/vertex.glsl", "shader/wireFrame/fragment.glsl") });
	shaders.insert({ RenderMode::BlinnPhong, std::make_shared<Shader>("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl") });
	//PBR‘› ±Œ¥ µœ÷
	shaders.insert({ RenderMode::PBR, std::make_shared<Shader>("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl") });
	shaders.insert({ RenderMode::Depth, std::make_shared<Shader>("shader/depthRender/vertex.glsl", "shader/depthRender/fragment.glsl") });
	shaders.insert({ RenderMode::Normal, std::make_shared<Shader>("shader/normal/vertex.glsl", "shader/normal/fragment.glsl") });
}

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

#pragma region TEST
//void TestBox(const Shader& program)
//{
//	//Materials
//	BlinnPhongMaterial gold{ glm::vec3(0.75164, 0.60648, 0.22648), glm::vec3(0.628281, 0.555802, 0.366065), glm::vec3(0.24725,0.1995,0.0745), 0.4f };
//	BlinnPhongMaterial silver{ glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), glm::vec3(0.19225f, 0.19225f, 0.19225f), 51.2f };
//
//	std::vector<float> vertices = {
//			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
//			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
//			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
//			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
//			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
//
//			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
//			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
//			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
//			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
//			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
//
//			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
//			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
//			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
//			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
//
//			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
//			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
//			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
//
//			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
//			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
//			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
//
//			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
//			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
//			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
//			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
//			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
//	};
//
//	VertexArray vao;
//	vao.bind();
//	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
//	VertexDataLayout layout;
//	layout.push<float>(3);
//	layout.push<float>(3);
//	layout.push<float>(2);
//	vao.AddBuffer(vbo, layout);
//	vao.unbind();
//
//	setLight(program);
//
//	Texture texture1("resource/textures/dogface.jpg", TEXTURE_TYPE::DIFFUSE);
//	Texture texture2("resource/textures/container.jpg", TEXTURE_TYPE::DIFFUSE);
//
//	vao.bind();
//
//	texture1.bind();
//	texture2.bind();
//
//	program.bind();
//
//	//uniforms
//
//
//	program.setVec3("material.diffuseColor", gold.getDiffuseColor());
//	program.setVec3("material.specularColor", gold.getSpecularColor());
//	program.setVec3("material.ambientColor", gold.getAmbientColor());
//	program.setFloat("material.shininess", gold.getShininess());
//}
#pragma endregion
