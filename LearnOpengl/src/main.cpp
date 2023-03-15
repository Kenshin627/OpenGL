#include <glad/gl.h>
#include <glfw3.h>
#include <iostream>
#include <array>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexDataLayout.h"
#include "IndexBuffer.h"

using std::cout;
using std::endl;

GLFWwindow* initWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void render(GLFWwindow* window);
void processInput(GLFWwindow* window);
VertexArray prepareData();
unsigned shaderCompile();

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

VertexArray prepareData()
{
	std::array<float, 12> vertices = { 0.5f,  0.5f, 0.0f,  // top right
									   0.5f, -0.5f, 0.0f,  // bottom right
									  -0.5f, -0.5f, 0.0f,  // bottom left
									  -0.5f,  0.5f, 0.0f   // top left 
	};
	std::array<unsigned, 6> indices = { 0, 1, 3,   // first triangle
										1, 2, 3    // second triangle
	};

	VertexArray vao;
	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
	VertexDataLayout layout;
	layout.push<float>(3);
	vao.AddBuffer(vbo, layout);
	IndexBuffer ibo{ indices.data(), indices.size() };
	ibo.bind();
	return vao;
}

unsigned shaderCompile()
{
	//VERTEX
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success;
	std::array<char, 512> infoLog;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog.data());
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog.data() << endl;
	}

	//FRAGMENT
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 color;\n"
		"void main()\n"
		"{\n"
		"FragColor = color;\n"
		"}\n";
	unsigned fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog.data());
		cout << "ERROR:SHADER::FRAGMENT::COMPLILATION_FAILED\n" << infoLog.data() << endl;
	}

	//Program
	unsigned program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
		cout << "ERROR:PROGRAM:: LINK_FAILED\n" << infoLog.data() << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}

void render(GLFWwindow* window)
{
	//VertexArray vao = prepareData();

	std::array<float, 12> vertices = { 0.5f,  0.5f, 0.0f,  // top right
									   0.5f, -0.5f, 0.0f,  // bottom right
									  -0.5f, -0.5f, 0.0f,  // bottom left
									  -0.5f,  0.5f, 0.0f   // top left 
	};
	std::array<unsigned, 6> indices = { 0, 1, 3,   // first triangle
										1, 2, 3    // second triangle
	};

	VertexArray vao;
	vao.bind();
	VertexBuffer vbo{ vertices.data(), sizeof(float) * vertices.size() };
	VertexDataLayout layout;
	layout.push<float>(3);
	vao.AddBuffer(vbo, layout);
	IndexBuffer ibo{ indices.data(), indices.size() };
	vao.unbind();

	unsigned program = shaderCompile();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		float time = glfwGetTime();
		float green = (sin(time) / 2.0) + 0.5;
		auto colorLocation = glGetUniformLocation(program, "color");

		/**------------**/
		glUseProgram(program);
		glUniform4f(colorLocation, 0.0, green, 1.0, 1.0);
		vao.bind();
		ibo.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}