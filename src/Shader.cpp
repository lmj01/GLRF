#include <GLRF/Shader.hpp>

using namespace GLRF;

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

void Shader::setValue(const std::string & name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setValue(const std::string & name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setValue(const std::string & name, unsigned int value) const {
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setValue(const std::string & name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setValue(const std::string & name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setValue(const std::string & name, glm::mat3 value) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setValue(const std::string & name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setValue(const std::string & name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setValue(const std::string& name, glm::vec2 value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec4> material_property, size_t texture_unit) {
	setValue(name + period + value_default, material_property.value_default);
	setValue(name + period + use_texture,	material_property.texture.has_value());
	setValue(name + period + texture,		static_cast<unsigned int>(texture_unit));
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec3> material_property, size_t texture_unit) {
	setValue(name + period + value_default, material_property.value_default);
	setValue(name + period + use_texture, material_property.texture.has_value());
	setValue(name + period + texture, static_cast<unsigned int>(texture_unit));
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec2> material_property, size_t texture_unit) {
	setValue(name + period + value_default, material_property.value_default);
	setValue(name + period + use_texture, material_property.texture.has_value());
	setValue(name + period + texture, static_cast<unsigned int>(texture_unit));
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<float> material_property, size_t texture_unit) {
	setValue(name + period + value_default, material_property.value_default);
	setValue(name + period + use_texture, material_property.texture.has_value());
	setValue(name + period + texture, static_cast<unsigned int>(texture_unit));
}

void Shader::setMaterial(const std::string & name, Material material) {
	material.bindTextures(0);
	setMaterialProperty(name + period + "albedo",		material.albedo,	0);
	setMaterialProperty(name + period + "normal",		material.normal,	1);
	setMaterialProperty(name + period + "roughness",	material.roughness, 2);
	setMaterialProperty(name + period + "metallic",		material.metallic,	3);
	setMaterialProperty(name + period + "ao",			material.ao,		4);
	setMaterialProperty(name + period + "height",		material.height,	5);

	setValue(name + period + "height_scale", material.height_scale);
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