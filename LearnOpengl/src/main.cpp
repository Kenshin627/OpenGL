#include <glad/gl.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <numeric>

#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/VertexDataLayout.h"
#include "Buffer/IndexBuffer.h"
#include "program/Shader.h"
#include "Texture/Texture.h"
#include "camera/camera.h"
#include "light/directionLight.h"
#include "material/BlinnPhongMaterial.h"
#include "light/pointLight.h"
#include "light/spotLight.h"
#include "sceneLoader/sceneLoader.h"

using std::cout;
using std::endl;


static int SCR_WIDTH = 800;
static int SCR_HEIGHT = 600;
static bool isFirst = true;
static float last_mouseX = SCR_WIDTH / 2;
static float last_mouseY = SCR_HEIGHT / 2;
static bool isHoldRightBtn = false;

GLFWwindow* initWindow(int width, int height);
//��������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//����ƶ�
void mouseMove_callback(GLFWwindow* window, double xpos, double ypos);
//��갴��
void mouseClick_callback(GLFWwindow* window, int button, int action, int modes);
void render(GLFWwindow* window, const SceneGraph& sceneGraph);
//����
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void clear();

SceneLoader loader;

//camera
Camera camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3{ 0,1,0 }, 800.0f / 600.0f, 0.1f, 100.0f, glm::radians(45.0f), 1.0f, 0.01);

//Lights
DirectionLight dl{ glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f) };
PointLight pl{ glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(1.0f), 50 };
SpotLight sl{ camera.getPosition(), camera.getForward(), glm::vec3(1.0f), 20.0f, 2.5f, 6.0f };

//Materials
BlinnPhongMaterial gold{ glm::vec3(0.75164, 0.60648, 0.22648), glm::vec3(0.628281, 0.555802, 0.366065), glm::vec3(0.24725,0.1995,0.0745), 0.4f };
BlinnPhongMaterial silver{ glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), glm::vec3(0.19225f, 0.19225f, 0.19225f), 51.2f };

int main()
{
	GLFWwindow* window = initWindow(SCR_WIDTH, SCR_HEIGHT);

	SceneGraph sceneGraph;	
	auto root = loader.loadModel("resource/models/nanosuit/nanosuit.obj");
	if (root)
	{
		sceneGraph.roots.push_back(root);
	}
	render(window, sceneGraph);
}

GLFWwindow* initWindow(int width, int height)
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(width, height, "openGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "create glfw window failed" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		cout << "failed to initialize GLAD!" << endl;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, height, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseMove_callback);
	glfwSetMouseButtonCallback(window, mouseClick_callback);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const auto keyConfig = camera.getKeybordConfig();
	for (auto& keyV : keyConfig)
	{
		if (glfwGetKey(window, keyV.first) == GLFW_PRESS)
		{
			camera.move(keyV.second, deltaTime);
		}
	}
}

void mouseMove_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isHoldRightBtn)
	{
		if (isFirst)
		{
			last_mouseX = xpos;
			last_mouseY = ypos;
			isFirst = false;
		}
		float xoffset = xpos - last_mouseX;
		float yoffset = ypos - last_mouseY;
		last_mouseX = xpos;
		last_mouseY = ypos;
		camera.pitchYaw(xoffset, yoffset);
	}	
}

void mouseClick_callback(GLFWwindow* window, int button, int action, int modes)
{
	isHoldRightBtn = button == 1 && action == 1 ? true : false;
}

void render(GLFWwindow* window, const SceneGraph& sceneGraph)
{
	std::cout << camera << std::endl;
	//VertexArray vao = prepareData();
	std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	VertexArray vao;
	vao.bind();
	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
	VertexDataLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);
	vao.AddBuffer(vbo, layout);
	vao.unbind();

	Texture texture1("resource/textures/dogface.jpg", TEXTURE_TYPE::DIFFUSE);
	Texture texture2("resource/textures/container.jpg", TEXTURE_TYPE::DIFFUSE);
	Shader program("shader/blinnPhong/vertex.glsl", "shader/blinnPhong/fragment.glsl");

	vao.bind();

	texture1.bind();
	texture2.bind();

	program.bind();
	
	//uniforms
	program.setVec3("directionLight.color", dl.getColor());
	program.setVec3("directionLight.direction", dl.getDirection());

	program.setVec3("pointLight.color", pl.getColor());
	program.setVec3("pointLight.position", pl.getPosition());
	program.setFloat("pointLight.kc", pl.getAttenuation().kc);
	program.setFloat("pointLight.kl", pl.getAttenuation().kl);
	program.setFloat("pointLight.kd", pl.getAttenuation().kd);

	program.setVec3("spotLight.color", sl.getColor());
	program.setVec3("spotLight.position", sl.getPosition());
	program.setVec3("spotLight.direction", sl.getDirection());
	program.setFloat("spotLight.kc", sl.getAttenuation().kc);
	program.setFloat("spotLight.kl", sl.getAttenuation().kl);
	program.setFloat("spotLight.kd", sl.getAttenuation().kd);
	program.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(sl.getInnerCutOff())));
	program.setFloat("spotLight.outterCutOff", glm::cos(glm::radians(sl.getOutterCutOff())));

	program.setVec3("material.diffuseColor", gold.getDiffuseColor());
	program.setVec3("material.specularColor", gold.getSpecularColor());
	program.setVec3("material.ambientColor", gold.getAmbientColor());
	program.setFloat("material.shininess", gold.getShininess());	
	
	/**enable-------------------------------------------*/
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/**-------------------------------------------------*/

	/**test---------------------------------------------*/

	/**-------------------------------------------------*/
	float currentTime = 0.0f;
	float lastTime	  = 0.0f;
	float deltaTime	  = 0.0f;
	float rotateangle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		rotateangle += 0.1;
		currentTime = glfwGetTime();
		deltaTime   = currentTime - lastTime;
		lastTime    = currentTime;
		processInput(window, camera, deltaTime);
		clear();
		
		/**-----DRAW CALL-------------------------------**/
		glm::mat4x4 modelMatrix;
		modelMatrix = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(rotateangle), glm::vec3(0, 1, 0));
		//glm::rotate(modelMatrix, glm::radians(45), glm::vec3(1, 0, 0));
		glm::mat3x3 modelInverseTranspose = glm::mat3x3(glm::transpose(glm::inverse(modelMatrix)));
		program.setVec3("cameraPosition", camera.getPosition());
		program.setMat4x4("modelViewProjection", camera.projMatrix() * camera.viewMatrix() * modelMatrix);
		program.setMat4x4("model", modelMatrix);
		program.setMat3x3("modelInverseTranspose", modelInverseTranspose);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/**-----DRAW CALL END---------------------------- */

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void clear()
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}