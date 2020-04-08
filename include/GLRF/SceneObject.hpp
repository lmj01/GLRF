#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

#include "VertexFormat.hpp"
#include "Material.hpp"

class SceneMesh {
public:
	SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material);
	SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType);
	void update(std::vector<VertexFormat> vertices, GLenum drawType);
	void update(std::vector<VertexFormat> vertices);
	void draw();
	Material getMaterial();
	void setMaterial(Material material);
private:
	GLuint VBO, VAO;
	GLenum drawType;
	GLenum geometryType;
	std::vector<VertexFormat> vertices;
	Material material;
	void setUp(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material);
};

class SceneMeshNode {
public:
	SceneMeshNode(SceneMesh * mesh);
	SceneMesh * getMesh();
	void setPosition(glm::vec3 position);
	void setRotation(glm::mat4 rotation);
	void setPositionAndRotation(glm::vec3 position, glm::mat4 rotation);
	void move(glm::vec3 offset);
	void rotate(glm::vec3 axis, float degrees);
	void moveAndRotate(glm::vec3 offset, glm::vec3 rotationAxis, float degrees);
	void recalculateMatrices();
	glm::vec3 getPosition();
	glm::mat4 getRotation();
	glm::mat4 getModelMatrix();
private:
	SceneMesh * mesh = nullptr;
	glm::vec3 position = glm::vec3(0.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f), rotation = glm::mat4(1.0f);
};