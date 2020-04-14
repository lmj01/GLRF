#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <GLRF/VertexFormat.hpp>
#include <GLRF/VectorMath.hpp>

/**
 * @brief A generator for 3d planes as in flat surfaces.
 * 
 */
class PlaneGenerator {
public:
	/**
	 * @brief Creates a new plane. The form is that of a square.
	 * 
	 * @param center the origin of the plane
	 * @param normal the surface normal of the plane
	 * @param direction one axis direction of the plane
	 * @param side_length the length of all four sides of the plane
	 * @param tesselation how many subdivisions the plane consists of
	 * @param uvScaling a factor to modify the appearance through the use of textures (should be > 0, default is 1)
	 * @return std::vector<VertexFormat> a vector containing all points (and their mappings) of the plane
	 */
	std::vector<VertexFormat> create(glm::vec3 center, glm::vec3 normal, glm::vec3 direction, float side_length, unsigned int tesselation, float uvScaling);
};