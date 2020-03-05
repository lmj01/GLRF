#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Material.h"

struct ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
};

struct ShaderOptions {
	bool isOneDimensionalFilterKernel = false;
	bool useDepthBuffer = true;
	bool useFrameBuffer = false;
	bool isFrameBufferHDR = false;
	bool useMultipleFrameBuffers = false;
	unsigned int texColorBufferAmount = 1;
	ScreenResolution screenResolution;
};

class Shader
{
public:
	GLuint ID;
	ShaderOptions shaderOptions;

	// constructor reads and builds the shader
	Shader(const std::string shaderLib, const std::string vertexPath, const std::string fragmentPath, ShaderOptions shaderOptions);

	// use/activate the shader
	void use();

	// returns the shader-program identifier
	unsigned int getProgramID();

	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setUInt(const std::string &name, unsigned int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;
	void setMat3(const std::string &name, glm::mat3 value) const;
	void setVec4(const std::string &name, glm::vec4 value) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setMaterial(const std::string &name, Material material);

	GLuint getFrameBuffer(unsigned int index);
	GLuint getColorBuffer(unsigned int index);
private:
	static const unsigned int MAX_FRAMEBUFFERS = 16;
	GLuint depthBuffer = 0;
	std::vector<GLuint> frameBuffers, texColorBuffers;

	void setUpFrameBuffer();
};

#endif