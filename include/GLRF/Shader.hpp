#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLRF/Material.hpp>

namespace GLRF {
	struct ScreenResolution;
	struct ShaderOptions;
	class Shader;
}

/**
 * @brief A screen resolution consisting of width and height parameters.
 * 
 */
struct GLRF::ScreenResolution {
	unsigned int width = 800;
	unsigned int height = 600;
	ScreenResolution(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;
	}
	ScreenResolution() {
		ScreenResolution(800, 600);
	}
};

/**
 * @brief Options for a Shader that specify its behaviour.
 * 
 * Core aspects are screen resolution, buffers, HDR and post-processing effects.
 */
struct GLRF::ShaderOptions {
	bool isOneDimensionalFilterKernel = false;
	bool useDepthBuffer = true;
	bool useFrameBuffer = false;
	bool isFrameBufferHDR = false;
	bool useMultipleFrameBuffers = false;
	unsigned int texColorBufferAmount = 1;
	ScreenResolution screenResolution;
};

/**
 * @brief The shader that is used by OpenGL to render a Scene.
 * 
 */
class GLRF::Shader {
public:
	GLuint ID;
	ShaderOptions shaderOptions;

	/**
	 * @brief Construct a new Shader object.
	 * 
	 * @param shaderLib the relative path to a collection of shaders
	 * @param vertexPath the relative path to the GLSL vertex shader
	 * @param fragmentPath the relative path to the GLSL fragment shader
	 * @param shaderOptions the options that modify the behaviour of the shader
	 * 
	 * Creates a new Shader from the specified library path and sub-paths to the vertex and fragment shader files.
	 * Takes shader options as input to configure itself.
	 */
	Shader(const std::string shaderLib, const std::string vertexPath, const std::string fragmentPath, ShaderOptions shaderOptions);

	/**
	 * @brief Activates the shader for rendering.
	 * 
	 */
	void use();

	/**
	 * @brief Returns the shader-program identifier.
	 * 
	 */
	unsigned int getProgramID();

	// === utility uniform functions ===

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, bool value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, int value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, unsigned int value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, float value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, glm::mat4 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, glm::mat3 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, glm::vec4 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setValue(const std::string& name, glm::vec3 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setMaterial(const std::string &name, Material material);

	/**
	 * @brief Get the FrameBuffer object at the specified index.
	 * 
	 * @param index the position of the FrameBuffer
	 * @return GLuint the number that refers to the OpenGL unit associated with the FrameBuffer
	 */
	GLuint getFrameBuffer(unsigned int index);

	/**
	 * @brief Get the ColorBuffer object at the specified index.
	 * 
	 * @param index the position of the ColorBuffer
	 * @return GLuint the number that refers to the OpenGL unit associated with the ColorBuffer
	 */
	GLuint getColorBuffer(unsigned int index);
private:
	static const char period = '.';
	static const unsigned int MAX_FRAMEBUFFERS = 16;
	GLuint depthBuffer = 0;
	std::vector<GLuint> frameBuffers, texColorBuffers;

	void setUpFrameBuffer();
};