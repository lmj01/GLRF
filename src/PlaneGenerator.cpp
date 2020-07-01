#include <iostream>
#include <GLRF/PlaneGenerator.hpp>

using namespace GLRF;

std::shared_ptr<MeshData<VertexFormat>> PlaneGenerator::create(glm::vec3 center, glm::vec3 normal,
	glm::vec3 direction, float side_length, unsigned int  tesselation, float uvScaling) {
	std::shared_ptr<MeshData<VertexFormat>> data = std::shared_ptr<MeshData<VertexFormat>>(new MeshData<VertexFormat>());
	unsigned int steps = tesselation + 1;
	float step_size = side_length / steps;
	float step_size_uv = 1.0f / (float)steps;
	data->vertices = std::vector<VertexFormat>();
	data->indices = std::vector<GLuint>();
	size_t steps_size_t = static_cast<size_t>(steps);
	data->vertices.reserve(steps_size_t * steps_size_t); // #steps vertices per step in each dimension
	data->indices.value().reserve(steps_size_t * steps_size_t * static_cast<size_t>(6)); // 2 triangles per step in each dimension
	glm::vec3 r = glm::normalize(glm::cross(normal, direction));
	glm::vec3 next_row = r * step_size;
	glm::vec3 next_column = direction * step_size;
	glm::vec3 start = center - (side_length / 2) * (direction + r);

	glm::vec3 tangent = r;

	unsigned int steps_vertices = steps + 1;
	for (unsigned int s = 0; s < steps_vertices; s++) {
		for (unsigned int t = 0; t < steps_vertices; t++) {
			float s_f = static_cast<float>(s);
			float t_f = static_cast<float>(t);
			glm::vec3 p = start + next_row * s_f + next_column * t_f;
			glm::vec2 uv = glm::vec2(s_f, t_f) * step_size_uv;
			data->vertices.push_back(VertexFormat(p, normal, uv * uvScaling, tangent));
		}
	}

	for (unsigned int s = 0; s < steps; s++) {
		for (unsigned int t = 0; t < steps; t++) {
			GLuint start_idx = steps_vertices * s + t;
			data->indices.value().push_back(start_idx);
			data->indices.value().push_back(start_idx + 1);
			data->indices.value().push_back(start_idx + steps_vertices);

			data->indices.value().push_back(start_idx + steps_vertices);
			data->indices.value().push_back(start_idx + 1);
			data->indices.value().push_back(start_idx + steps_vertices + 1);
		}
	}

	return data;
}