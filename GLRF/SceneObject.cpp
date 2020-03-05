#include "SceneObject.h"

SceneMesh::SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material) {
	setUp(vertices, drawType, geometryType, material);
}

SceneMesh::SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType) {
	Material material;
	setUp(vertices, drawType, geometryType, material);
}

void SceneMesh::draw() {
	glBindVertexArray(VAO);

	glDrawArrays(geometryType, 0, vertices.size());

	glBindVertexArray(0);
}

Material SceneMesh::getMaterial() {
	return this->material;
}

void SceneMesh::setMaterial(Material material) {
	this->material = material;
}

void SceneMesh::setUp(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	this->drawType = drawType;
	this->geometryType = geometryType;
	this->vertices = vertices;
	this->material = material;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], drawType);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void SceneMesh::update(std::vector<VertexFormat> vertices, GLenum drawType) {
	this->vertices = vertices;
	this->drawType = drawType;
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), NULL, drawType);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], drawType);
}

void SceneMesh::update(std::vector<VertexFormat> vertices) {
	SceneMesh::update(vertices, drawType);
}

SceneMeshNode::SceneMeshNode(SceneMesh * mesh) {
	this->mesh = mesh;
}

SceneMesh * SceneMeshNode::getMesh() {
	return this->mesh;
}

void SceneMeshNode::setPosition(glm::vec3 position) {
	this->position = position;
	recalculateMatrices();
}

void SceneMeshNode::setRotation(glm::mat4 rotation) {
	this->rotation = rotation;
	recalculateMatrices();
}

void SceneMeshNode::setPositionAndRotation(glm::vec3 position, glm::mat4 rotation) {
	this->position = position;
	this->rotation = rotation;
	recalculateMatrices();
}

void SceneMeshNode::move(glm::vec3 offset) {
	this->position += offset;
	recalculateMatrices();
}

void SceneMeshNode::rotate(glm::vec3 axis, float degrees) {
	this->rotation = glm::rotate(this->rotation, glm::radians(degrees), axis);
	recalculateMatrices();
}

void SceneMeshNode::moveAndRotate(glm::vec3 offset, glm::vec3 rotationAxis, float degrees) {
	this->position += offset;
	this->rotation = glm::rotate(this->rotation, glm::radians(degrees), rotationAxis);
	recalculateMatrices();
}

void SceneMeshNode::recalculateMatrices() {
	this->modelMatrix = glm::translate(this->rotation, this->position);
}

glm::vec3 SceneMeshNode::getPosition() {
	return this->position;
}

glm::mat4 SceneMeshNode::getRotation() {
	return this->rotation;
}

glm::mat4 SceneMeshNode::getModelMatrix() {
	return this->modelMatrix;
}
