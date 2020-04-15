#pragma once
#include <glm/glm.hpp>

namespace GLRF {
	struct VertexFormat;
}

/**
 * @brief The format of a vertex with all related information
 * 
 */
struct GLRF::VertexFormat {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;

	/**
	 * @brief Construct a new VertexFormat object.
	 * 
	 * @param position the position of the vertex
	 * @param normal the normal vector at the vertex
	 * @param uv the uv-coordinate at the vertex
	 * @param tangent the tangent vector at the vertex
	 */
	VertexFormat(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv, const glm::vec3 &tangent);
};