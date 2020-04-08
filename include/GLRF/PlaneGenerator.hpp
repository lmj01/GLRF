#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "VertexFormat.hpp"
#include "VectorMath.hpp"

class PlaneGenerator {
public:
	std::vector<VertexFormat> create(glm::vec3 center, glm::vec3 normal, glm::vec3 direction, float side_length, unsigned int tesselation, float uvScaling);
};