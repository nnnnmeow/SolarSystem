#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
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



		unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
		unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	unsigned int getID() const {
		return ID;
	}

	~Shader() {
		glDeleteProgram(ID);
	}

	void use() const{
		glUseProgram(ID);
	}

	void setMat4(const char* name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat));
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