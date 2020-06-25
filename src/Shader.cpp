#include <GLRF/Shader.hpp>

#include <glm/gtx/string_cast.hpp>
#define PRINT(text) std::cout << text << std::endl;
#define PRINT_VAR(text, var) std::cout << text << " = " << var << std::endl;

using namespace GLRF;

unsigned int createShader(GLenum shaderType, const GLchar * shaderSource);

ShaderConfiguration::ShaderConfiguration()
{
	
}

ShaderConfiguration::~ShaderConfiguration()
{
	this->v_bool.clear();
	this->v_float.clear();
	this->v_int.clear();
	this->v_uint.clear();
	this->v_mat4.clear();
	this->v_mat3.clear();
	this->v_vec4.clear();
	this->v_vec3.clear();
	this->v_vec2.clear();
	this->v_material.clear();
}

void ShaderConfiguration::loadIntoShader(Shader * shader) const
{
	for (auto pair : this->v_bool)
	{
		shader->setBool(pair.first, pair.second);
	}
	for (auto pair : this->v_int)
	{
		shader->setInt(pair.first, pair.second);
	}
	for (auto pair : this->v_uint)
	{
		shader->setUInt(pair.first, pair.second);
	}
	for (auto pair : this->v_float)
	{
		shader->setFloat(pair.first, pair.second);
	}
	for (auto pair : this->v_mat4)
	{
		shader->setMat4(pair.first, pair.second);
	}
	for (auto pair : this->v_mat3)
	{
		shader->setMat3(pair.first, pair.second);
	}
	for (auto pair : this->v_vec4)
	{
		shader->setVec4(pair.first, pair.second);
	}
	for (auto pair : this->v_vec3)
	{
		shader->setVec3(pair.first, pair.second);
	}
	for (auto pair : this->v_vec2)
	{
		shader->setVec2(pair.first, pair.second);
	}
	for (auto pair : this->v_material)
	{
		shader->setMaterial(pair.first, pair.second);
	}
}

void ShaderConfiguration::setBool(const std::string& name, bool value)
{
	this->v_bool.insert({ name, value });
}

void ShaderConfiguration::setInt(const std::string& name, GLint value)
{
	this->v_int.insert({ name, value });
}

void ShaderConfiguration::setUInt(const std::string & name, GLuint value)
{
	this->v_uint.insert({ name, value });
}

void ShaderConfiguration::setFloat(const std::string & name, float value)
{
	this->v_float.insert({ name, value });
}

void ShaderConfiguration::setMat4(const std::string & name, glm::mat4 value)
{
	this->v_mat4.insert({ name, value });
}

void ShaderConfiguration::setMat3(const std::string & name, glm::mat3 value)
{
	this->v_mat3.insert({ name, value });
}

void ShaderConfiguration::setVec4(const std::string & name, glm::vec4 value)
{
	this->v_vec4.insert({ name, value });
}

void ShaderConfiguration::setVec3(const std::string & name, glm::vec3 value)
{
	this->v_vec3.insert({ name, value });
}

void ShaderConfiguration::setVec2(const std::string & name, glm::vec2 value)
{
	this->v_vec2.insert({ name, value });
}

void ShaderConfiguration::setMaterial(const std::string & name, std::shared_ptr<Material> material)
{
	this->v_material.insert({ name, material });
}


Shader::Shader(const std::string shader_lib, const std::string vertex_path, std::optional<const std::string> geometry_path,
		const std::string fragment_path)
{
	const bool has_geometry_shader = geometry_path.has_value();

	// 1. retrieve the GLSL source code from paths
	std::string vertex_lib_path = shader_lib + vertex_path;
	std::string vertex_code_str;
	loadShaderFile(vertex_lib_path, &vertex_code_str);
	const char* vertex_code = vertex_code_str.c_str();

	std::string geometry_lib_path;
	std::string geometry_code_str;
	const char* geometry_code;
	if (has_geometry_shader)
	{
		geometry_lib_path = shader_lib + geometry_path.value();
		loadShaderFile(geometry_lib_path, &geometry_code_str);
		geometry_code = geometry_code_str.c_str();
	}

	std::string fragment_lib_path = shader_lib + fragment_path;
	std::string fragment_code_str;
	loadShaderFile(fragment_lib_path, &fragment_code_str);
	const char* fragment_code = fragment_code_str.c_str();

	// 2. compile shaders
	GLuint vertex_id, geometry_id, fragment_id;
	int success;
	char infoLog[512];

	vertex_id = createShader(GL_VERTEX_SHADER, vertex_code, "VERTEX");
	if (has_geometry_shader) geometry_id = createShader(GL_GEOMETRY_SHADER, geometry_code, "GEOMETRY");
	fragment_id = createShader(GL_FRAGMENT_SHADER, fragment_code, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex_id);
	if (has_geometry_shader) glAttachShader(ID, geometry_id);
	glAttachShader(ID, fragment_id);
	glLinkProgram(ID);

	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		exit(1);
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex_id);
	if (has_geometry_shader) glDeleteShader(geometry_id);
	glDeleteShader(fragment_id);

	// ======= REGISTER SHADER ======= //
	ShaderManager::getInstance().registerShader(this);
}

void Shader::loadShaderFile(const std::string shader_path, std::string* out)
{
	std::ifstream file;
	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open file
		file.open(shader_path);
		std::stringstream stream;
		// read file's buffer contents into streams
		stream << file.rdbuf();
		// close file handlers
		file.close();
		// convert stream into string
		*out = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

unsigned int Shader::getID() {
	return this->ID;
}

void Shader::setBool(const std::string & name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, GLint value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUInt(const std::string & name, GLuint value) const {
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

void Shader::setVec2(const std::string& name, glm::vec2 value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec4> material_property, GLuint texture_unit) {
	setVec4(name + period + value_default,	material_property.value_default);
	setMaterialPropertyCommons(name, material_property, texture_unit);
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec3> material_property, GLuint texture_unit) {
	setVec3(name + period + value_default,	material_property.value_default);
	setMaterialPropertyCommons(name, material_property, texture_unit);
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<glm::vec2> material_property, GLuint texture_unit) {
	setVec2(name + period + value_default,	material_property.value_default);
	setMaterialPropertyCommons(name, material_property, texture_unit);
}

void Shader::setMaterialProperty(const std::string& name, MaterialProperty<float> material_property, GLuint texture_unit) {
	setFloat(name + period + value_default, material_property.value_default);
	setMaterialPropertyCommons(name, material_property, texture_unit);
}

void Shader::setMaterial(const std::string & name, std::shared_ptr<Material> material) {
	material->bindTextures(0);
	setMaterialProperty(name + period + "albedo",		material->albedo,		0);
	setMaterialProperty(name + period + "normal",		material->normal,		1);
	setMaterialProperty(name + period + "roughness",	material->roughness,	2);
	setMaterialProperty(name + period + "metallic",		material->metallic,		3);
	setMaterialProperty(name + period + "ao",			material->ao,			4);
	setMaterialProperty(name + period + "height",		material->height,		5);

	setFloat(name + period + "height_scale", material->height_scale);
}

unsigned int Shader::createShader(GLenum shader_type, const GLchar * shader_source, std::string shader_name) {
	unsigned int shader;
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shader_name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(1);
	}

	return shader;
}

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::registerShader(Shader * shader)
{
	this->registered_shaders.insert_or_assign(shader->getID(), shader);
}

void ShaderManager::useShader(GLuint ID)
{
	if (this->activeShaderID != ID)
	{
		glUseProgram(ID);
		this->activeShaderID = ID;
	}
}

void ShaderManager::configureShader(ShaderConfiguration * configuration, GLuint ID)
{
	if (this->configured_shaders.find(ID) == this->configured_shaders.end())
	{
		auto it = this->registered_shaders.find(ID);
		if (it == this->registered_shaders.end())
		{
			throw std::invalid_argument("shader was used but never registered");
		}
		configuration->loadIntoShader(it->second);
	}
}

void ShaderManager::clearDrawConfigurations()
{
	this->configured_shaders.clear();
}

Shader * ShaderManager::getShader(GLuint ID)
{
	return this->registered_shaders.find(ID)->second;
}

void Shader::setDebugName(const std::string name)
{
	this->debug_name = name;
}

std::string Shader::getDebugName()
{
	return this->debug_name;
}