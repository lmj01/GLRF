#include <GLRF/SceneObject.hpp>

using namespace GLRF;

SceneMesh::SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material) {
	setUp(vertices, drawType, geometryType, material);
}

SceneMesh::SceneMesh(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType) {
	Material material;
	setUp(vertices, drawType, geometryType, material);
}

void SceneMesh::draw() {
	glBindVertexArray(VAO);

	glDrawArrays(geometryType, 0, static_cast<GLsizei>(vertices.size()));

	glBindVertexArray(0);
}

void SceneMesh::setUp(std::vector<VertexFormat> vertices, GLenum drawType, GLenum geometryType, Material material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	this->drawType = drawType;
	this->geometryType = geometryType;
	this->vertices = vertices;
	setMaterial(material);

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