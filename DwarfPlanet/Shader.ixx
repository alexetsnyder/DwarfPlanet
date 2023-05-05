export module Shader;

#include <glad/glad.h>

import <iostream>;
import <fstream>;
import <string>;
import glmModule;

#define NULL 0

export class Shader
{
	public:
		Shader()
		{
			vertexShader = 0;
			fragmentShader = 0;
			shaderProgram = 0;
		}

		Shader(std::string vShaderPath, std::string fShaderPath)
		{
			setVertexShader(vShaderPath);
			setFragmentShader(fShaderPath);
		}

		void setVertexShader(std::string vShaderPath)
		{
			std::string shaderStr = loadFile(vShaderPath);
			const char* shaderSource = shaderStr.c_str();
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &shaderSource, NULL);
		}

		void setFragmentShader(std::string fShaderPath)
		{
			std::string shaderStr = loadFile(fShaderPath);
			const char* shaderSource = shaderStr.c_str();
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &shaderSource, NULL);
		}

		bool compile(char infoLog[512])
		{
			if (!compile(vertexShader, infoLog) ||
				!compile(fragmentShader, infoLog))
			{
				return false;
			}
			return true;
		}

		bool link(char infoLog[512])
		{
			int success;

			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				return false;
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return true;
		}

		void use()
		{
			glUseProgram(shaderProgram);
		} 

		void setUniform(std::string name, Vec4 v)
		{
			unsigned int uniformLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniform4fv(uniformLoc, 1, value_ptr(v));
		}

		void setUniform(std::string name, Mat4 m)
		{
			unsigned int uniformLoc = glGetUniformLocation(shaderProgram, name.c_str());
			glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value_ptr(m));
		}

	private:
		std::string loadFile(std::string filePath)
		{
			std::string fileStr = "";
			std::string line;
			std::ifstream file(filePath);

			if (file.is_open())
			{
				while (getline(file, line))
				{
					fileStr += line + "\n";
				}
				file.close();
			}
			else
			{
				std::cout << "Unable to open file " << filePath << std::endl;
			}

			return fileStr;
		}

		bool compile(unsigned int shader, char infoLog[512])
		{
			int success;
			glCompileShader(shader);
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				return false;
			}
			return true;
		}

		unsigned int shaderProgram;
		unsigned int vertexShader;
		unsigned int fragmentShader;
};
