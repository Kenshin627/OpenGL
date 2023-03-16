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

using std::cout;
using std::endl;


static int SCR_WIDTH = 800;
static int SCR_HEIGHT = 600;
static bool isFirst = true;
static float last_mouseX = SCR_WIDTH / 2;
static float last_mouseY = SCR_HEIGHT / 2;
static bool isHoldRightBtn = false;

GLFWwindow* initWindow(int width, int height);
//窗口缩放
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//鼠标移动
void mouseMove_callback(GLFWwindow* window, double xpos, double ypos);
//鼠标按键
void mouseClick_callback(GLFWwindow* window, int button, int action, int modes);
void render(GLFWwindow* window);
//键盘
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void clear();

//camera
Camera camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1),
	glm::vec3{ 0,1,0 },
	800.0f / 600.0f,
	0.1f,
	100.0f,
	glm::radians(45.0f),
	1.0f,
	0.01
);

int main()
{
	GLFWwindow* window = initWindow(SCR_WIDTH, SCR_HEIGHT);
	render(window);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void render(GLFWwindow* window)
{
	std::cout << camera << std::endl;
	//VertexArray vao = prepareData();
	std::vector<float> vertices = {    
										-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
										 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
										 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
										 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
										-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
										-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

										-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
										 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
										-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
										-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

										-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
										-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
										-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
										-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
										-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
										-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
										 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
										 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
										 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
										 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

										-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
										 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
										 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
										 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
										-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
										-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

										-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
										 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
										 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
										-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
										-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	                              };
	VertexArray vao;
	vao.bind();
	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
	VertexDataLayout layout;
	layout.push<float>(3);
	layout.push<float>(2);
	vao.AddBuffer(vbo, layout);
	vao.unbind();

	Texture texture1("resource/textures/dogface.jpg", 0);
	Texture texture2("resource/textures/container.jpg", 1);
	Shader program("shader/triangle/vertex.glsl", "shader/triangle/fragment.glsl");

	vao.bind();

	texture1.bind();
	texture2.bind();

	program.bind();
	
	program.setUniform1i("sampler1", 0);
	program.setUniform1i("sampler2", 1);
	
	/**enable-------------------------------------------*/
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	/**-------------------------------------------------*/

	/**test---------------------------------------------*/

	/**-------------------------------------------------*/
	float currentTime = 0.0f;
	float lastTime	  = 0.0f;
	float deltaTime	  = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		deltaTime   = currentTime - lastTime;
		lastTime    = currentTime;
		processInput(window, camera, deltaTime);
		clear();
		
		/**-----DRAW CALL-------------------------------**/
		program.setMat4x4("modelViewProjection", camera.projMatrix() * camera.viewMatrix());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/**-----DRAW CALL END---------------------------- */

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void clear()
{	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}