#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLRF/Material.hpp>

namespace GLRF {
	struct ScreenResolution;
	struct ShaderOptions;
	class ShaderConfiguration;
	class Shader;
	class ShaderManager;
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

class GLRF::ShaderConfiguration {
public:
	ShaderConfiguration();
	~ShaderConfiguration();

	void loadIntoShader(Shader * shader) const;

	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, GLint value);
	void setUInt(const std::string& name, GLuint value);
	void setFloat(const std::string& name, float value);
	void setMat4(const std::string& name, glm::mat4 value);
	void setMat3(const std::string& name, glm::mat3 value);
	void setVec4(const std::string& name, glm::vec4 value);
	void setVec3(const std::string& name, glm::vec3 value);
	void setVec2(const std::string& name, glm::vec2 value);
	void setMaterial(const std::string& name, std::shared_ptr<Material> material);
private:
	std::map<std::string, bool>			v_bool;
	std::map<std::string, int>			v_int;
	std::map<std::string, unsigned int> v_uint;
	std::map<std::string, float>		v_float;
	std::map<std::string, glm::mat4>	v_mat4;
	std::map<std::string, glm::mat3>	v_mat3;
	std::map<std::string, glm::vec4>	v_vec4;
	std::map<std::string, glm::vec3>	v_vec3;
	std::map<std::string, glm::vec2>	v_vec2;
	std::map<std::string, std::shared_ptr<Material>> v_material;
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
	void setBool(const std::string& name, bool value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setInt(const std::string& name, GLint value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setUInt(const std::string& name, GLuint value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setFloat(const std::string& name, float value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setMat4(const std::string& name, glm::mat4 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setMat3(const std::string& name, glm::mat3 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setVec4(const std::string& name, glm::vec4 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setVec3(const std::string& name, glm::vec3 value) const;

	/**
	 * @brief Sets the specified value for the specified, named variable in this Shader.
	 *
	 * @param name the name of the variable that will be set
	 * @param value the new value for the variable
	 */
	void setVec2(const std::string& name, glm::vec2 value) const;

	/**
	 * @brief Sets the specified material for the specified, named variable in this Shader.
	 * 
	 * @param name the name of the material that will be set
	 * @param material the new material for the variable
	 */
	void setMaterial(const std::string &name, std::shared_ptr<Material> material);

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
	const std::string value_default = "value_default";
	const std::string use_texture = "use_texture";
	const std::string texture = "texture";
	static const unsigned int MAX_FRAMEBUFFERS = 16;
	GLuint depthBuffer = 0;
	std::vector<GLuint> frameBuffers, texColorBuffers;

	void setUpFrameBuffer();

	/**
	 * @brief Sets the specified material property for the specified, named variable in this Shader.
	 *
	 * @param name the name of the material property that will be set
	 * @param material_property the new material property for the variable
	 *
	 * The 4-dimensional property will be set directly.
	 */
	void setMaterialProperty(const std::string& name, MaterialProperty<glm::vec4> material_property, GLuint texture_unit);

	/**
	 * @brief Sets the specified material property for the specified, named variable in this Shader.
	 *
	 * @param name the name of the material property that will be set
	 * @param material_property the new material property for the variable
	 *
	 * The 3-dimensional property will be set as 4-dimensional with an additional value of 1 at the end.
	 */
	void setMaterialProperty(const std::string& name, MaterialProperty<glm::vec3> material_property, GLuint texture_unit);

	/**
	 * @brief Sets the specified material property for the specified, named variable in this Shader.
	 *
	 * @param name the name of the material property that will be set
	 * @param material_property the new material property for the variable
	 *
	 * The 2-dimensional property will be set as 4-dimensional with two additional values of 1 at the end.
	 */
	void setMaterialProperty(const std::string& name, MaterialProperty<glm::vec2> material_property, GLuint texture_unit);

	/**
	 * @brief Sets the specified material property for the specified, named variable in this Shader.
	 *
	 * @param name the name of the material property that will be set
	 * @param material_property the new material property for the variable
	 *
	 * The 1-dimensional property will be copied to a 3-dimensional value and set as 4-dimensional
	 * with an additional value of 1 at the end.
	 */
	void setMaterialProperty(const std::string& name, MaterialProperty<float> material_property, GLuint texture_unit);

	/**
	 * @brief Sets common aspects of material properties.
	 */
	template <typename T>
	void setMaterialPropertyCommons(const std::string& name, MaterialProperty<T> material_property, GLuint texture_unit) {
		setBool(name + period + use_texture, material_property.texture.has_value());
		setInt(name + period + texture, texture_unit);
	}
};

class GLRF::ShaderManager
{
public:
	static ShaderManager& getInstance() {
		static ShaderManager instance;
		return instance;
	}

	~ShaderManager();

	void registerShader(Shader * shader);

	void useShader(GLuint ID);

	void configureShader(ShaderConfiguration * configuration, GLuint ID);

	void clearDrawConfigurations();

	Shader * getShader(GLuint ID);
private:
	std::map<GLuint, Shader *> registered_shaders;
	std::set<GLuint> configured_shaders;

	ShaderManager();
	ShaderManager(const ShaderManager&);
	ShaderManager& operator = (const ShaderManager&);
};