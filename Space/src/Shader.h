#include"Config.h"
#pragma once

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode, fragmentCode;
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			std::cerr << "Error: Could not open shader files." << std::endl;
			return;
		}

		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		vertexFile.close();
		fragmentFile.close();



		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use() const{
		glUseProgram(ID);
	}

	unsigned int compileShader(unsigned int type, const char* source) {
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		return shader;
	}
private:
	unsigned int ID;
};