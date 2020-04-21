#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <VectorMath.hpp>
#include <SceneObject.hpp>

namespace GLRF {
	class PointLight;
	class DirectionalLight;
}

/**
 * @brief A point lightsource. Light is emitted in all directions away from the specified origin.
 * 
 */
class GLRF::PointLight {
public:
	/**
	 * @brief Construct a new PointLight object.
	 * 
	 * @param color the color of the lightsource
	 * @param power the brightness of the emitted light
	 */
	PointLight(glm::vec3 color, float power = 1.f);

	/**
	 * @brief Returns the color of the PointLight.
	 * 
	 * @return glm::vec3 the color of the PointLight
	 */
	glm::vec3 getColor();

	/**
	 * @brief Returns the brightness of the emitted light of the PointLight.
	 * 
	 * @return glm::vec3 the brightness of the emitted light of the PointLight
	 */
	float getPower();
private:
	glm::vec3 color;
	float power;
};

/**
 * @brief A directional lightsource. Light is emitted in the specified direction only.
 * 
 */
class GLRF::DirectionalLight {
public:
	/**
	 * @brief Construct a new DirectionalLight object.
	 * 
	 * @param power the brightness of the emitted light
	 */
	DirectionalLight(float power = 1.f);

	/**
	 * @brief Returns the light emitting direction.
	 * 
	 * @return glm::vec3 the light emitting direction
	 */
	glm::vec3 getDirection();

	/**
	 * @brief Returns the brightness of the emitted light of the PointLight.
	 * 
	 * @return glm::vec3 the brightness of the emitted light of the PointLight
	 */
	float getPower();
private:
	float power;
};