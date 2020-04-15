#pragma once
#include <vector>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <VertexFormat.hpp>
#include <Material.hpp>

namespace GLRF {
	class SceneMesh;
	class SceneMeshNode;
}

/**
 * @brief A mesh of vertices that resembles a 3d object.
 * 
 * The data is stored here and should not be copied.
 * Instead, a new SceneMeshNode should be created that only points to the mesh, thus reducing memory usage.
 */
class GLRF::SceneMesh {
public:
	/**
	 * @brief Construct a new SceneMesh object.
	 * 
	 * @param vertices the vertices that define the structure of the mesh
	 * @param drawType the OpenGL draw type that specifies how the mesh will be rendered
	 * @param geometryType the type of primitive that will be drawn (e.g. triangles) for the meshs vertices
	 * @param material the material that defines the appearance of the mesh
	 */
	SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material);

	/**
	 * @brief Construct a new SceneMesh object. A default material is used.
	 * 
	 * @param vertices the vertices that define the structure of the mesh
	 * @param drawType the OpenGL draw type that specifies how the mesh will be rendered
	 * @param geometryType the type of primitive that will be drawn (e.g. triangles) for the meshs vertices
	 */
	SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType);

	/**
	 * @brief Updates the vertex data and the draw type of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 * @param drawType the new value for the OpenGL draw type
	 */
	void update(std::vector<VertexFormat> vertices, GLenum drawType);

	/**
	 * @brief Updates the vertex data of the mesh.
	 * 
	 * @param vertices the new vertices that will replace the old vertices
	 */
	void update(std::vector<VertexFormat> vertices);

	/**
	 * @brief Draws the mesh with the current shader.
	 * 
	 */
	void draw();

	/**
	 * @brief Returns the Material object.
	 * 
	 * @return Material the material of the mesh
	 */
	Material getMaterial();

	/**
	 * @brief Sets the Material object.
	 * 
	 * @param material the new material for the mesh
	 */
	void setMaterial(Material material);
private:
	GLuint VBO, VAO;
	GLenum drawType;
	GLenum geometryType;
	std::vector<VertexFormat> vertices;
	Material material;
	void setUp(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material);
};

/**
 * @brief A lightweight reference / instance of a specific mesh that buffers changes to the meshs local coordinate system.
 * 
 */
class GLRF::SceneMeshNode {
public:
	/**
	 * @brief Construct a new SceneMeshNode object.
	 * 
	 * @param mesh the mesh that will be referenced
	 */
	SceneMeshNode(std::shared_ptr<SceneMesh> mesh);

	/**
	 * @brief Get the Mesh object that this node refers to.
	 * 
	 * @return std::shared_ptr<SceneMesh> a shared pointer to the mesh of this node
	 */
	std::shared_ptr<SceneMesh> getMesh();

	/**
	 * @brief Sets the position of this node. Use 'move' for relative displacement.
	 * 
	 * @param position the position to be set
	 */
	void setPosition(glm::vec3 position);

	/**
	 * @brief Sets the rotation of this node. Use 'rotate' for relative rotation.
	 * 
	 * @param rotation the rotation to be set
	 */
	void setRotation(glm::mat4 rotation);

	/**
	 * @brief Sets the position and rotation of this node.
	 * 
	 * @param position the position to be set
	 * @param rotation the rotation to be set
	 */
	void setPositionAndRotation(glm::vec3 position, glm::mat4 rotation);

	/**
	 * @brief Moves the node along a vector. Use 'setPosition' for absolute displacement.
	 * 
	 * @param offset the vector to move along at
	 */
	void move(glm::vec3 offset);

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in degrees
	 */
	void rotateDeg(glm::vec3 axis, float angle);

	/**
	 * @brief Rotates the node around an axis. Use 'setRotation' for absolute rotation.
	 * 
	 * @param axis the axis to rotate around
	 * @param angle the angle in radians
	 */
	void rotateRad(glm::vec3 axis, float angle);

	/**
	 * @brief Moves and rotates the node.
	 * 
	 * @param offset the vector to move along at
	 * @param rotationAxis the axis to rotate around
	 * @param angle the angle in degrees
	 */
	void moveAndRotateDeg(glm::vec3 offset, glm::vec3 rotationAxis, float angle);

	/**
	 * @brief Moves and rotates the node.
	 * 
	 * @param offset the vector to move along at
	 * @param rotationAxis the axis to rotate around
	 * @param angle the angle in radians
	 */
	void moveAndRotateRad(glm::vec3 offset, glm::vec3 rotationAxis, float angle);

	/**
	 * @brief Recalculates the matrices that define the instances local coordinate system.
	 * 
	 */
	void recalculateMatrices();

	/**
	 * @brief Returns the position vector.
	 * 
	 * @return glm::vec3 the position vector
	 */
	glm::vec3 getPosition();

	/**
	 * @brief Returns the rotation vector.
	 * 
	 * @return glm::vec3 the rotation vector
	 */
	glm::mat4 getRotation();

	/**
	 * @brief Returns the model matrix.
	 * 
	 * @return glm::vec3 the model matrix
	 */
	glm::mat4 getModelMatrix();
private:
	std::shared_ptr<SceneMesh> mesh = nullptr;
	glm::vec3 position = glm::vec3(0.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f), rotation = glm::mat4(1.0f);
};