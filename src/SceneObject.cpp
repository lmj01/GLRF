#include <GLRF/SceneObject.hpp>

using namespace GLRF;

SceneMesh::SceneMesh(MeshData data, GLenum drawType, Material material) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	this->drawType = drawType;
	this->geometryType = GL_TRIANGLES;
	this->data.vertices = data.vertices;
	this->data.indices = data.indices;
	setMaterial(material);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data.vertices.size(), &this->data.vertices[0], drawType);

	if (this->data.indices.has_value()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data.indices.value().size(), &this->data.indices.value()[0], drawType);
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(8 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void SceneMesh::draw() {
	glBindVertexArray(VAO);

	if (data.indices.has_value()) {
		glDrawElements(GL_TRIANGLES, data.indices.value().size(), GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(geometryType, 0, static_cast<GLsizei>(data.vertices.size()));
	}

	glBindVertexArray(0);
}

void SceneMesh::update(MeshData data, GLenum drawType) {
	this->data.vertices = data.vertices;
	this->drawType = drawType;
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data.vertices.size(), NULL, drawType);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data.vertices.size(), &this->data.vertices[0], drawType);
	if (!this->data.indices.has_value() && data.indices.has_value()) {
		// do not use indices -> use indices
		this->data.indices = data.indices;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data.indices.value().size(), &this->data.indices.value()[0], drawType);
	} else if (this->data.indices.has_value() && !data.indices.has_value()) {
		// use indices -> do not use indices
		this->data.indices = std::nullopt;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data.indices.value().size(), NULL, drawType);
	}
}

void SceneMesh::update(MeshData data) {
	SceneMesh::update(data, drawType);
}