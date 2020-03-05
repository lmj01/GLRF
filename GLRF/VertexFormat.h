#pragma once
#include "glm\glm.hpp"

struct VertexFormat {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;

	VertexFormat(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv, const glm::vec3 &tangent);
};