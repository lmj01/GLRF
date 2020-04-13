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

#include <GLRF/Material.hpp>

/**
 * A screen resolution consisting of width and height parameters.
 */
struct ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
};

/**
 * Options for a Shader that specify its behaviour.
 * Core aspects are screen resolution, buffers, HDR and post-processing effects.
 */
struct ShaderOptions {
	bool isOneDimensionalFilterKernel = false;
	bool useDepthBuffer = true;
	bool useFrameBuffer = false;
	bool isFrameBufferHDR = false;
	bool useMultipleFrameBuffers = false;
	unsigned int texColorBufferAmount = 1;
	ScreenResolution screenResolution;
};

/**
 * The shader that is used by OpenGL to render a Scene.
 */
class Shader
{
public:
	GLuint ID;
	ShaderOptions shaderOptions;

	/**
	 * Creates a new Shader from the specified library path and sub-paths to the vertex and fragment shader files.
	 * Takes shader options as input to configure itself.
	 */
	Shader(const std::string shaderLib, const std::string vertexPath, const std::string fragmentPath, ShaderOptions shaderOptions);

	/**
	 * Activates the shader for rendering.
	 */
	void use();

	/**
	 * Returns the shader-program identifier.
	 */
	unsigned int getProgramID();

	// utility uniform functions

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, bool value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, int value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, unsigned int value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, float value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, glm::mat4 value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, glm::mat3 value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, glm::vec4 value) const;

	/**
	 * Sets the specified value for the specified, named variable in this Shader.
	 */
	void setValue(const std::string& name, glm::vec3 value) const;

	/**
	 * Sets a complete material in the shader as a struct with the specified name.
	 */
	void setMaterial(const std::string &name, Material material);

	/**
	 * Returns the FrameBuffer at the specified index.
	 */
	GLuint getFrameBuffer(unsigned int index);

	/**
	 * Returns the ColorBuffer at the specified index.
	 */
	GLuint getColorBuffer(unsigned int index);
private:
	static const char period = '.';
	static const unsigned int MAX_FRAMEBUFFERS = 16;
	GLuint depthBuffer = 0;
	std::vector<GLuint> frameBuffers, texColorBuffers;

	void setUpFrameBuffer();
};

#endif