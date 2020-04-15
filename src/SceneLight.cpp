#include <SceneLight.hpp>
#include <iostream>

using namespace GLRF;

PointLight::PointLight(glm::vec3 position, glm::vec3 color, float power) {
	this->position = position;
	this->power = power;
	this->color = color;
}

void PointLight::translate(glm::vec3 offset) {
	this->position += offset;
}

glm::vec3 PointLight::getPosition() {
	return this->position;
}

glm::vec3 PointLight::getColor() {
	return this->color;
}

float PointLight::getPower() {
	return this->power;
}

DirectionalLight::DirectionalLight(float phi, float theta, float power) {
	this->phi = fmod(phi, phi_max);
	this->theta = glm::clamp(theta, theta_min, theta_max);
	this->power = power;
}

void DirectionalLight::rotate(float d_phi, float d_theta) {
	this->phi = fmod(phi + d_phi, phi_max);
	this->theta = glm::clamp(theta + d_theta, theta_min, theta_max);
}

glm::vec3 DirectionalLight::getDirection() {
	const float phi_rad = glm::radians(phi);
	const float theta_rad = glm::radians(theta);
	return glm::vec3(
		cos(phi_rad) * sin(theta_rad),
		cos(theta_rad),
		sin(phi_rad) * sin(theta_rad)
	);
}

float DirectionalLight::getPower() {
	return this->power;
}
