#include <GLRF/VertexFormat.hpp>

using namespace GLRF;

VertexFormat::VertexFormat(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec2 & uv, const glm::vec3 &tangent) {
	this->position = position;
	this->normal = normal;
	this->uv = uv;
	this->tangent = tangent;
}

VertexFormat::~VertexFormat()
{

}

void VertexFormat::registerFormat()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(8 * sizeof(GLfloat)));
}