#include "SceneLight.hpp"
#include <iostream>

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

DirectionalLight::DirectionalLight(float xDegree, float yDegree, float power) {
	this->xDegreeTotal = xDegree;
	this->yDegreeTotal = yDegree;
	this->power = power;
}

void DirectionalLight::rotate(float xDegree, float yDegree) {
	this->xDegreeTotal += xDegree;
	this->yDegreeTotal += yDegree;
	glm::mat4 xRot = glm::rotate(this->rotationMatrix, glm::radians(xDegree), glm::vec3(1.0f, 0.0f, 0.0f));
	this->rotationMatrix = glm::rotate(xRot, glm::radians(yDegree), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 DirectionalLight::getDirection() {
	glm::vec4 direction(0.0f, -1.0f, 0.0f, 0.0f);
	direction = this->rotationMatrix * direction;
	glm::vec3 dirDehom = dehomogenizeVec4(direction);
	return glm::normalize(dirDehom);
}

float DirectionalLight::getPower() {
	return this->power;
}
