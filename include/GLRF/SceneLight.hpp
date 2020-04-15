#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLRF/VectorMath.hpp>

/**
 * @brief A point lightsource. Light is emitted in all directions away from the specified origin.
 * 
 */
class PointLight {
public:
	/**
	 * @brief Construct a new PointLight object.
	 * 
	 * @param position the position of the lightsource
	 * @param color the color of the lightsource
	 * @param power the brightness of the emitted light
	 */
	PointLight(glm::vec3 position, glm::vec3 color, float power = 1.f);

	/**
	 * @brief Moves the PointLight along a given offset vector.
	 * 
	 * @param offset the offset to add to the current position
	 */
	void translate(glm::vec3 offset);

	/**
	 * @brief Returns the position of the PointLight.
	 * 
	 * @return glm::vec3 the position of the PointLight
	 */
	glm::vec3 getPosition();

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
	glm::vec3 position, color;
	float power;
};

/**
 * @brief A directional lightsource. Light is emitted in the specified direction only.
 * 
 * The direction of the lightsource is expressed with two angles, theta and phi
 * that correspond to the rotation of the direction of the light in spherecoordinates.
 */
class DirectionalLight {
public:
	/**
	 * @brief Construct a new DirectionalLight object.
	 * 
	 * @param phi the angle (in degrees) around the y-axis starting at x=1 and x=0
	 * @param theta the angle (in degrees) from +y to -y
	 * @param power the brightness of the emitted light
	 */
	DirectionalLight(float phi = 0.f, float theta = 180.f, float power = 1.f);

	/**
	 * @brief Rotates the light emitting direction.
	 * 
	 * @param d_phi the offset of the phi angle (in degrees)
	 * @param d_theta the offset of the theta angle (in degrees)
	 */
	void rotate(float d_phi, float d_theta);

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
	float phi = 0.0f;	// min 0�		max 180�
	const float phi_max = 360.f;
	float theta = 0.0f;	// min 0�		max 360�
	const float theta_min = 0.f;
	const float theta_max = 180.f;
};