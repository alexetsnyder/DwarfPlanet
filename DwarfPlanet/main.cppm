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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;

Camera camera(Vec3(0.0f, 17.0f, 0.0f));

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
