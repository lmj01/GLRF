#include <GLRF/SceneObject.hpp>

using namespace GLRF;

MeshData::MeshData()
{

}

MeshData::~MeshData()
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

SceneMesh::SceneMesh(std::shared_ptr<MeshData> data, GLenum drawType, std::shared_ptr<Material> material)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	this->drawType = drawType;
	this->geometryType = GL_TRIANGLES;
	this->data = data;
	setMaterial(material);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data->vertices.size(), &this->data->vertices[0], drawType);

	if (this->data->indices.has_value()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data->indices.value().size(), &this->data->indices.value()[0], drawType);
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

SceneMesh::~SceneMesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void SceneMesh::draw()
{
	glBindVertexArray(VAO);

	if (data->indices.has_value()) {
		glDrawElements(GL_TRIANGLES, data->indices.value().size(), GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(geometryType, 0, static_cast<GLsizei>(data->vertices.size()));
	}

	glBindVertexArray(0);
}

void SceneMesh::update(std::shared_ptr<MeshData> data, GLenum drawType)
{
	this->data = data;
	this->drawType = drawType;
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data->vertices.size(), NULL, drawType);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->data->vertices.size(), &this->data->vertices[0], drawType);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->data->indices.value().size(),
		(data->indices.has_value()) ? &this->data->indices.value()[0] : NULL, drawType);
}

void SceneMesh::update(std::shared_ptr<MeshData> data)
{
	SceneMesh::update(data, this->drawType);
}