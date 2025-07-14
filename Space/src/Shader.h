#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <gl/GLU.h>
#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath, const char* texturePath) {
		this->texturePath = texturePath;

		textureID = LoadTextureFromFile();

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);

		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			std::cerr << "Error: Could not open shader files." << std::endl;
			ID = 0;
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

		int VertexSuccess;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &VertexSuccess);
		if (!VertexSuccess)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "vertex ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		int FragmentSuccess;
		char infoLog2[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &FragmentSuccess);
		if (!FragmentSuccess)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
			std::cerr << "fragment ERROR::SHADER::COMPILATION_FAILED\n" << infoLog2 << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		int linkSuccess;
		char infoLog3[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &linkSuccess);
		if (!linkSuccess) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog3);
			ID = 0;
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog3 << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (ID == 0) {
			std::cerr << "Shader program failed to compile/link." << std::endl;
		}
		std::cout << "Shader ID = " << ID << std::endl;
	}

	GLuint LoadTextureFromFile()
	{
		int width, height, channels;
		unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);
		if (!data) {
			std::cerr << "Failed to load image: " << texturePath << "\n";
			return 0;
		}

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = GL_RGB;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		return textureID;
	}

	unsigned int getID() const {
		return ID;
	}

	~Shader() {
		glDeleteProgram(ID);
	}

	void use() const{
		glUseProgram(ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	GLint getUniformLocation(const std::string& name) const {
		return glGetUniformLocation(ID, name.c_str());
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(getUniformLocation(name), value);
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
	const char* texturePath;
	GLuint textureID;
};