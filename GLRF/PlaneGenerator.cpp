#include "PlaneGenerator.hpp"
#include <iostream>

std::vector<VertexFormat> PlaneGenerator::create(glm::vec3 center, glm::vec3 normal, glm::vec3 direction, float side_length, unsigned int  tesselation, float uvScaling) {
	std::vector<VertexFormat> result;
	unsigned int  steps = tesselation + 1;
	float step_size = (tesselation != 0) ? side_length / steps : side_length;
	float step_size_uv = 1.0f / (float)steps;
	result.reserve(steps * steps * 6); // 2 triangles per step in each dimension
	glm::vec3 r = glm::normalize(glm::cross(normal, direction));
	glm::vec3 next_row = r * step_size;
	glm::vec3 next_column = direction * step_size;
	glm::vec3 start = center - (side_length / 2) * (direction + r);

	glm::vec3 tangent = r;

	glm::vec3 current_offset_row(0.0, 0.0, 0.0);
	for (unsigned int s = 0; s < steps; s++) {
		glm::vec3 current_offset_column(0.0, 0.0, 0.0);
		for (unsigned int t = 0; t < steps; t++) {
			glm::vec3 p1 = start + current_offset_row + current_offset_column;
			glm::vec3 p2 = p1 + next_column;
			glm::vec3 p3 = p1 + next_row;
			glm::vec3 p4 = p3 + next_column;

			glm::vec2 uv1 = glm::vec2((float) s, (float) t) * step_size_uv;
			glm::vec2 uv2 = uv1 + glm::vec2(0.0f, step_size_uv);
			glm::vec2 uv3 = uv1 + glm::vec2(step_size_uv, 0.0f);
			glm::vec2 uv4 = uv3 + glm::vec2(0.0f, step_size_uv);

			result.push_back(VertexFormat(p1, normal, uv1 * uvScaling, tangent));
			result.push_back(VertexFormat(p2, normal, uv2 * uvScaling, tangent));
			result.push_back(VertexFormat(p3, normal, uv3 * uvScaling, tangent));

			result.push_back(VertexFormat(p3, normal, uv3 * uvScaling, tangent));
			result.push_back(VertexFormat(p2, normal, uv2 * uvScaling, tangent));
			result.push_back(VertexFormat(p4, normal, uv4 * uvScaling, tangent));

			current_offset_column += next_column;
		}
		current_offset_row += next_row;
	}

	return result;
}