#include <GLRF/VertexFormat.hpp>

VertexFormat::VertexFormat(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec2 & uv, const glm::vec3 &tangent) {
	this->position = position;
	this->normal = normal;
	this->uv = uv;
	this->tangent = tangent;
}
