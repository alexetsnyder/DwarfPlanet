export module Window;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

import <iostream>;
import <string>;

export class Window
{
	public:
		Window(std::string name, int width, int height)
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = NULL;
			this->name = name;
			this->width = width;
			this->height = height;
		}

		bool initGLFW()
		{
			window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
			if (window == NULL)
			{
				std::cout << "Failed to create GLFW window\n";
				return false;
			}
			glfwMakeContextCurrent(window);

			return true;
		}

		bool initGLAD()
		{
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to load GLAD\n";
				return false;
			}

			return true;
		}

		GLFWwindow* getWindow()
		{
			return window;
		}

		bool isRunning()
		{
			return !glfwWindowShouldClose(window);
		}

		void disableCursor()
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		void swapBuffers()
		{
			glfwSwapBuffers(window);
		}

		void pollEvents()
		{
			glfwPollEvents();
		}

		void free()
		{
			glfwTerminate();
		}

	private:
		GLFWwindow* window;
		int width, height;
		std::string name;
};