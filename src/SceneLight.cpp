#include <iostream>
#include <GLRF/SceneLight.hpp>

using namespace GLRF;

PointLight::PointLight(glm::vec3 color, float power) {
	this->power = power;
	this->color = color;
}

glm::vec3 PointLight::getColor() {
	return this->color;
}

float PointLight::getPower() {
	return this->power;
}

DirectionalLight::DirectionalLight(float power) {
	this->power = power;
}

glm::vec3 DirectionalLight::getDirection() {
	return glm::vec3(0, -1, 0);
}

float DirectionalLight::getPower() {
	return this->power;
}
