#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VectorMath.hpp"

/**
	A point lightsource. Light is emitted in all directions away from the specified origin.
*/
class PointLight {
public:
	PointLight(glm::vec3 position, glm::vec3 color, float power);
	void translate(glm::vec3 offset);
	glm::vec3 getPosition();
	glm::vec3 getColor();
	float getPower();
private:
	glm::vec3 position, color;
	float power = 1.0f;
};

/**
	A directional lightsource. Light is emitted in the specified direction only.
	The direction of the lightsource is expressed with two angles, theta and phi
	that correspond to the rotation of the direction of the light in spherecoordinates.
	Theta is the angle from +Y to -Y.
	Phi is the angle around the Y-axis starting at X=1 and Z=0.
*/
class DirectionalLight {
public:
	DirectionalLight(float phi, float theta, float power);
	void rotate(float d_phi, float d_theta);
	glm::vec3 getDirection();
	float getPower();
private:
	float power = 1.0f;
	float phi = 0.0f;	// min 0°		max 180°
	const float phi_max = 360.f;
	float theta = 0.0f;	// min 0°		max 360°
	const float theta_min = 0.f;
	const float theta_max = 180.f;
};