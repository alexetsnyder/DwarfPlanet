#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

import <iostream>;
import Window;
import glmModule;
import Camera;
import Shader;
import WorldGeneration;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

unsigned int createCube();
unsigned int createTexture();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;

Camera camera(Vec3(0.0f, 0.0f, 3.0f));

int main()
{
	Window window("Dwarf Planet", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!window.initGLFW())
	{
		window.free();
		return -1;
	}

	if (!window.initGLAD())
	{
		window.free();
		return -1;
	}

	window.disableCursor();
	glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	glfwSetScrollCallback(window.getWindow(), scroll_callback);

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shader shader("vShader.glsl", "fShader.glsl");

	char infoLog[512];
	if (!shader.compile(infoLog))
	{
		cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	if (!shader.link(infoLog))
	{
		cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << endl;
		return -1;
	}

	WorldGeneration worldGen;
	worldGen.generateWorld();

	unsigned int VAO = createCube();
	unsigned int texture = createTexture();

	Mat4 model = Mat4(1.0f);
	Mat4 view = Mat4(1.0f);
	Mat4 projection = Mat4(1.0f);

	while (window.isRunning())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.getWindow());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.getViewMatrix();
		projection = perspective(radians(camera.getZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

		shader.use();
		shader.setUniform("view", view);
		shader.setUniform("projection", projection);
		
		worldGen.draw(shader);

		glBindTexture(GL_TEXTURE_2D, texture);

		shader.setUniform("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		window.swapBuffers();
		window.pollEvents();
	}

	window.free();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.rotate(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.zoom(yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.move(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.move(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.move(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.move(RIGHT, deltaTime);
	}
}

float cubeVerticies[] =
{
	//top face
	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,  //0
	0.0f, 1.0f, 1.0f,   0.0f, 1.0f,	 //1
	1.0f, 1.0f, 0.0f,   1.0f, 0.0f,  //2
	1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  //3

	//bottom face
	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,  //4
	0.0f, 0.0f, 0.0f,	0.0f, 1.0f,	 //5
	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,  //6
	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,  //7

	//front face
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f,  //8
	0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  //9
	1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  //10
	1.0f, 1.0f, 0.0f,   1.0f, 1.0f,  //11

	//back face
	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,  //12
	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,  //13
	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,  //14
	0.0f, 1.0f, 1.0f,	1.0f, 1.0f,  //15

	//left face
	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,  //16
	0.0f, 1.0f, 1.0f,   0.0f, 1.0f,  //17
	0.0f, 0.0f, 0.0f,   1.0f, 0.0f,  //18
	0.0f, 1.0f, 0.0f,   1.0f, 1.0f,  //19

	//right face
	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,  //20
	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,  //21
	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,  //22
	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,  //23
};
unsigned int cubeIndices[] =
{
	 0,  1,  2,  2,  1,  3, //Top Face
	 4,  5,  6,  6,  5,  7, //Bottom Face
	 8,  9, 10, 10,  9, 11, //Front Face
	12, 13, 14, 14, 13, 15, //Back Face
	16, 17, 18, 18, 17, 19, //Left Face
	20, 21, 22, 22, 21, 23, //Right Face
};

unsigned int createCube()
{
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	for (int i = 0; i < 120; i += 5)
	{
		for (int j = i; j < i + 3; j++)
		{
			cubeVerticies[j] = cubeVerticies[j] - 0.5f;
		}
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticies), cubeVerticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

unsigned int createTexture()
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Assets/Textures/dirt.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture image\n";
	}
	stbi_image_free(data);

	return texture;
}