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
#include <GLRF/FrameBuffer.hpp>

namespace GLRF {
	struct ShaderOptions;
	class ShaderConfiguration;
	class Shader;
	class ShaderManager;
}

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
	/**
	 * @brief Construct a new Shader object.
	 * 
	 * @param shader_lib the relative path to a collection of shaders
	 * @param vertex_path the relative path to the GLSL vertex shader
	 * @param geometry_path the relative path to the GLSL geometry shader
	 * @param fragment_path the relative path to the GLSL fragment shader
	 * @param shader_options the options that modify the behaviour of the shader
	 * 
	 * Creates a new Shader from the specified library path and sub-paths to the vertex and fragment shader files.
	 * Takes shader options as input to configure itself.
	 */
	Shader(const std::string shader_lib, const std::string vertex_path, std::optional<const std::string> geometry_path,
		const std::string fragment_path);

	/**
	 * @brief Returns the shader-program identifier.
	 * 
	 */
	unsigned int getID();

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

	void setDebugName(const std::string name);
	std::string getDebugName();

private:
	static const char period = '.';
	const std::string value_default = "value_default";
	const std::string use_texture = "use_texture";
	const std::string texture = "texture";
	GLuint ID;
	std::string debug_name;

	unsigned int createShader(GLenum shader_type, const GLchar* shader_source, std::string shader_name);

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

	void loadShaderFile(const std::string shader_path, std::string * out);
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

	void configureShader(const ShaderConfiguration * configuration, GLuint ID, bool force);

	void clearDrawConfigurations();

	Shader * getShader(GLuint ID);
private:
	std::map<GLuint, Shader *> registered_shaders;
	std::set<GLuint> configured_shaders;
	GLuint activeShaderID = 0;

	ShaderManager();
	ShaderManager(const ShaderManager&);
	ShaderManager& operator = (const ShaderManager&);
};