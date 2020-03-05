#include "Shader.h"

unsigned int createShader(GLenum shaderType, const GLchar * shaderSource);

Shader::Shader(const std::string shaderLib, const std::string vertexPath, const std::string fragmentPath, ShaderOptions shaderOptions) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexLibPath = shaderLib + vertexPath;
	std::string fragmentLibPath = shaderLib + fragmentPath;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexLibPath);
		fShaderFile.open(fragmentLibPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = createShader(GL_VERTEX_SHADER, vShaderCode);

	// similiar for Fragment Shader
	fragment = createShader(GL_FRAGMENT_SHADER, fShaderCode);

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	this->shaderOptions = shaderOptions;
	if (this->shaderOptions.useMultipleFrameBuffers) this->shaderOptions.useFrameBuffer = true;
	if (shaderOptions.useFrameBuffer) setUpFrameBuffer();
}

void Shader::use() {
	glUseProgram(ID);
}

unsigned int Shader::getProgramID() {
	return ID;
}

void Shader::setBool(const std::string & name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUInt(const std::string & name, unsigned int value) const {
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string & name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string & name, glm::mat3 value) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec4(const std::string & name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string & name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMaterial(const std::string & name, Material material) {
	setVec3(name + ".albedo", material.albedo);
	setFloat(name + ".roughness", material.roughness);
	setFloat(name + ".metallic", material.metallic);
	setFloat(name + ".ao", material.ao);
	setBool(name + ".useTextureAlbedo", material.useTextureAlbedo);
	setBool(name + ".useTextureNormal", material.useTextureNormal);
	setBool(name + ".useTextureRoughness", material.useTextureRoughness);
	setBool(name + ".useTextureMetallic", material.useTextureMetallic);
	setBool(name + ".useTextureAo", material.useTextureAo);
	material.bindTextures(0);
	setInt(name + ".textureAlbedo", 0);
	setInt(name + ".textureNormal", 1);
	setInt(name + ".textureRoughness", 2);
	setInt(name + ".textureMetallic", 3);
	setInt(name + ".textureAo", 4);
}

GLuint Shader::getFrameBuffer(unsigned int index) {
	return this->frameBuffers[index];
}

GLuint Shader::getColorBuffer(unsigned int index) {
	return this->texColorBuffers[index];
}

void Shader::setUpFrameBuffer() {
	const bool useMultipleFrameBuffers = this->shaderOptions.useMultipleFrameBuffers;
	const unsigned int bufferAmount = this->shaderOptions.texColorBufferAmount;
	this->frameBuffers.resize(bufferAmount);
	this->texColorBuffers.resize(bufferAmount);
	glGenFramebuffers(useMultipleFrameBuffers ? bufferAmount : 1, &(this->frameBuffers[0]));
	glGenTextures(bufferAmount, &(this->texColorBuffers[0]));
	if (!useMultipleFrameBuffers) glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffers[0]);

	const unsigned int screenWidth = this->shaderOptions.screenResolution.width;
	const unsigned int screenHeight = this->shaderOptions.screenResolution.height;

	std::vector<GLenum> attachments;
	attachments.reserve(useMultipleFrameBuffers ? bufferAmount : 1);

	GLenum internalformat = GL_RGB;
	GLenum type = GL_UNSIGNED_BYTE;
	if (this->shaderOptions.isFrameBufferHDR) {
		internalformat = GL_RGB16F;
		type = GL_FLOAT;
	}

	for (unsigned int i = 0; i < bufferAmount; i++) {
		if (useMultipleFrameBuffers) glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffers[i]);
		glBindTexture(GL_TEXTURE_2D, this->texColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, screenWidth, screenHeight, 0, GL_RGB, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, useMultipleFrameBuffers ? GL_COLOR_ATTACHMENT0 : GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->texColorBuffers[i], 0);
		if (!useMultipleFrameBuffers) attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	if (this->shaderOptions.useDepthBuffer) {
		glGenRenderbuffers(1, &(this->depthBuffer));
		glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);

		glDrawBuffers(bufferAmount, attachments.data());
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int createShader(GLenum shaderType, const GLchar * shaderSource) {
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << *shaderSource << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}