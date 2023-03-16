#include <glad/gl.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexDataLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

using std::cout;
using std::endl;

GLFWwindow* initWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void render(GLFWwindow* window);
void processInput(GLFWwindow* window);

void clear();

int main()
{
	GLFWwindow* window = initWindow();
	render(window);
}

GLFWwindow* initWindow()
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "openGL", nullptr, nullptr);
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void render(GLFWwindow* window)
{
	//VertexArray vao = prepareData();
	std::vector<float> vertices = {    
		                               0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
									   0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom right
									  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
									  -0.5f,  0.5f, 0.0f, 0.0f, 1.0f// top left 
	                              };
	std::vector<unsigned> indices = {  
									   0, 1, 3,   // first triangle
									   1, 2, 3    // second triangle
	                                };

	VertexArray vao;
	vao.bind();
	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
	VertexDataLayout layout;
	layout.push<float>(3);
	layout.push<float>(2);
	vao.AddBuffer(vbo, layout);
	IndexBuffer ibo{ indices.data(), indices.size() };
	vao.unbind();

	Texture texture1("resource/textures/dogface.jpg", 0);
	Texture texture2("resource/textures/container.jpg", 1);

	Shader program("shader/triangle/vertex.glsl", "shader/triangle/fragment.glsl");

	vao.bind();
	ibo.bind();

	texture1.bind();
	texture2.bind();

	program.bind();
	
	program.setUniform1i("sampler1", 0);
	program.setUniform1i("sampler2", 1);
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		clear();
		
		/**-----DRAW CALL-------------------------------**/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/**-----DRAW CALL END-------------------------------**/

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}

void clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}