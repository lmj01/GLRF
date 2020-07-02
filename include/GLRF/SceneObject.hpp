#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLRF/VertexFormat.hpp>
#include <GLRF/Material.hpp>
#include <GLRF/IdManager.hpp>
#include <GLRF/Shader.hpp>

namespace GLRF {
	template <typename T> struct MeshData;
	class SceneObject;
	template <typename T> class SceneMesh;
	template <typename T> class SceneNode;
}

template <typename T>
class GLRF::MeshData {
public:
	MeshData()
	{
		
	}

	~MeshData()
	{
		this->vertices.clear();
		this->vertices.shrink_to_fit();
		if (this->indices.has_value())
		{
			auto indices_vector = this->indices.value();
			indices_vector.clear();
			indices_vector.shrink_to_fit();
		}
	}

	std::vector<T> vertices;
	std::optional<std::vector<GLuint>> indices = std::nullopt;
	void unionize(MeshData<T>& other) {
		size_t current_vertices_size = this->vertices.size();
		size_t current_indices_size = this->indices.value().size();
		size_t other_vertices_size = other.vertices.size();
		size_t other_indices_size = other.indices.value().size();

		bool this_has_indices = this->indices.has_value();
		bool other_has_indices = other.indices.has_value();

		if (this_has_indices == other_has_indices) {
			this->vertices.reserve(current_vertices_size + other_vertices_size);
			std::copy(other.vertices.begin(), other.vertices.end(), std::back_inserter(this->vertices));

			if (this_has_indices && other_has_indices) {
				this->indices.value().reserve(current_indices_size + other_indices_size);
				std::transform(other.indices.value().begin(), other.indices.value().end(), std::back_inserter(this->indices.value()),
					[current_vertices_size](GLuint x) -> GLuint { return x + current_vertices_size; });
			}
		}
	}
private:
};

/**
 * @brief A 3d object.
 * 
 * The data is stored here and should not be copied.
 * Instead, a new SceneNode should be created that only points to the mesh, thus reducing memory usage.
 */
class GLRF::SceneObject {
public:
	void setDebugName(const std::string name)
	{
		this->debug_name = name;
	}

	std::string getDebugName()
	{
		return this->debug_name;
	}

	void setShaderID(GLuint ID)
	{
		this->ID = ID;
	}

	GLuint getShaderID()
	{
		return this->ID;
	}


	void configureShader(ShaderConfiguration* scene_configuration, ShaderConfiguration* object_configuration)
	{
		GLuint shader_id = this->getShaderID();
		ShaderManager& shader_manager = ShaderManager::getInstance();
		shader_manager.useShader(shader_id);
		shader_manager.configureShader(scene_configuration, shader_id, false);
		shader_manager.configureShader(object_configuration, shader_id, true);
	}

	/**
	 * @brief Draws the object with the current shader.
	 * 
	 */
	virtual void draw(ShaderConfiguration* scene_configuration, ShaderConfiguration* object_configuration) = 0;

	/**
	 * @brief Returns the Material object.
	 * 
	 * @return Material the material of the object
	 */
	virtual std::shared_ptr<Material> getMaterial() { return this->material; }

	/**
	 * @brief Sets the Material object.
	 * 
	 * @param material the new material for the object
	 */
	virtual void setMaterial(std::shared_ptr<Material> material) { this->material = material; }

private:
	std::shared_ptr<Material> material;
	GLuint ID = 0;
	std::string debug_name = "MISSING_NAME";
};

/**
 * @brief A mesh of vertices that resembles a 3d object.
 * 
 * The data is stored here and should not be copied.
 * Instead, a new SceneNode should be created that only points to the mesh, thus reducing memory usage.
 */
template <typename T>
class GLRF::SceneMesh : public virtual SceneObject {
public:
	typedef T vertex_format_t;

	/**
	 * @brief Construct a new SceneMesh object.
	 * 
	 * @param vertices the vertices that define the structure of the mesh
	 * @param drawType the OpenGL draw type that specifies how the mesh will be rendered - e.g. GL_STATIC_DRAW
	 * @param material the material that defines the appearance of the mesh
	 */
	SceneMesh(std::shared_ptr<MeshData<T>> data, GLenum draw_type, GLenum geometry_type = GL_TRIANGLES,
		std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material()))
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		this->draw_type = draw_type;
		this->geometry_type = geometry_type;
		this->data = data;
		setMaterial(material);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * this->data->vertices.size(), &this->data->vertices[0], draw_type);

		if (this->data->indices.has_value()) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data->indices.value().size(), &this->data->indices.value()[0], draw_type);
		}

		vertex_format_t::registerFormat();

		glBindVertexArray(0);
	}

	~SceneMesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	/**
	 * @brief Updates the vertex data and the draw type of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 * @param drawType the new value for the OpenGL draw type
	 */
	void update(std::shared_ptr<MeshData<T>> data, GLenum draw_type, GLenum geometry_type)
	{
		this->data = data;
		this->draw_type = draw_type;
		this->geometry_type = geometry_type;

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * this->data->vertices.size(), NULL, draw_type);
		glBufferData(GL_ARRAY_BUFFER, sizeof(T) * this->data->vertices.size(), &this->data->vertices[0], draw_type);

		bool has_indices = data->indices.has_value();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, has_indices ? sizeof(GLuint) * this->data->indices.value().size() : 0,
			has_indices ? &this->data->indices.value()[0] : NULL, draw_type);

		glBindVertexArray(0);
	}

	/**
	 * @brief Updates the vertex data of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 */
	void update(std::shared_ptr<MeshData<T>> data)
	{
		SceneMesh::update(data, this->draw_type, this->geometry_type);
	}

	/**
	 * @brief Draws the mesh with the current shader.
	 * 
	 */
	void draw(ShaderConfiguration* scene_configuration, ShaderConfiguration* object_configuration)
	{
		configureShader(scene_configuration, object_configuration);

		glBindVertexArray(VAO);

		switch (this->geometry_type)
		{
		case GL_POINTS:
			glPointSize(8.f);
			break;
		case GL_LINES:
		case GL_LINE_STRIP:
		case GL_LINES_ADJACENCY:
		case GL_LINE_STRIP_ADJACENCY:
			glLineWidth(3.f);
			break;
		default:
			break;
		}

		if (data->indices.has_value()) {
			glDrawElements(this->geometry_type, data->indices.value().size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(this->geometry_type, 0, static_cast<GLsizei>(data->vertices.size()));
		}

		glBindVertexArray(0);
	}

private:
	GLuint VBO, VAO, EBO;
	GLenum draw_type;
	GLenum geometry_type;
	std::shared_ptr<MeshData<T>> data;
};

/**
 * @brief A lightweight reference / instance of a specific object that buffers changes to the meshs local coordinate system.
 * 
 */
template <typename T>
class GLRF::SceneNode {
public:
	const IdSpaceSize id;

	/**
	 * @brief Construct a new SceneNode object.
	 * 
	 * @param object the object that will be referenced
	 */
	SceneNode(std::shared_ptr<T> object) : id(IdManager::getInstance().getNodeId()) {
		this->object = object;
	}

	/**
	 * @brief Get the object that this node refers to.
	 * 
	 * @return std::shared_ptr<T> a shared pointer to the object of this node
	 */
	std::shared_ptr<T> getObject() {
		return this->object;
	}

	friend bool operator==(const SceneNode &n1, const SceneNode &n2) {
		return n1.id == n2.id;
	}

	friend bool operator!=(const SceneNode &n1, const SceneNode &n2) {
		return !(n1 == n2);
	}

	/**
	 * @brief Sets the position of this node. Use 'move' for relative displacement.
	 * 
	 * @param position the position to be set
	 */
	void setPosition(glm::vec3 position)  {
		this->position = position;
	}

	/**
	 * @brief Sets the rotation of this node. Use 'rotate' for relative rotation.
	 * 
	 * @param rotation the rotation to be set
	 */
	void setRotation(glm::mat4 rotation)  {
		this->rotation = rotation;
	}

	/**
	 * @brief Moves the node along a vector. Use 'setPosition' for absolute displacement.
	 * 
	 * @param offset the vector to move along at
	 */
	void move(glm::vec3 offset)  {
		this->position += offset;
	}

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in degrees
	 */
	void rotateDeg(glm::vec3 axis, float angle) {
		this->rotation = glm::rotate(this->rotation, glm::radians(angle), axis);
	}

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in radians
	 */
	void rotateRad(glm::vec3 axis, float angle) {
		this->rotation = glm::rotate(this->rotation, angle, axis);
	}

	/**
	 * @brief Returns the position vector.
	 * 
	 * @return glm::vec3 the position vector
	 */
	glm::vec3 getPosition() { return this->position; }

	/**
	 * @brief Returns the rotation vector.
	 * 
	 * @return glm::vec3 the rotation vector
	 */
	glm::mat4 getRotation() { return this->rotation; }

	/**
	 * @brief Calculates and returns the model matrix.
	 * 
	 * @return glm::vec3 the model matrix
	 */
	glm::mat4 calculateModelMatrix() {
		return glm::translate(glm::mat4(1.f), this->position) * this->rotation;
	}
private:
	std::shared_ptr<T> object = nullptr;
	glm::vec3 position = glm::vec3(0.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
};