#include <SceneCamera.hpp>

using namespace GLRF;

SceneCamera::SceneCamera(glm::vec3 position, glm::vec3 upVector, glm::vec3 target) {
	this->position = position;
	this->upVector = glm::normalize(upVector);
	this->w = glm::normalize(position - target);
	float cos_pitch = glm::dot(this->upVector, getV());
	this->pitch = -glm::degrees(acos(cos_pitch));
}

void SceneCamera::rotate(float yaw_offset, float pitch_offset, float sensitivity) {
	yaw_offset *= sensitivity * sensitivity_factor_rotation;
	pitch_offset *= sensitivity * sensitivity_factor_rotation;
	this->pitch = glm::clamp(this->pitch + pitch_offset, -80.f, 80.f);
	this->yaw += yaw_offset;
	float cos_pitch = cos(glm::radians(-this->pitch));
	float sin_pitch = sin(glm::radians(-this->pitch));

	glm::vec3 direction = glm::vec3(0.0f);
	direction.x = cos_pitch * cos(glm::radians(this->yaw));
	direction.y = sin_pitch;
	direction.z = cos_pitch * sin(glm::radians(this->yaw));
	this->w = glm::normalize(direction);
}

void SceneCamera::translate(glm::vec3 direction, float sensitivity) {
	this->position += direction * sensitivity * sensitivity_factor_translation;
}

glm::mat4 SceneCamera::getViewMatrix() {
	return glm::lookAt(this->position, this->position - this->w, this->upVector);
}

glm::vec3 SceneCamera::getPosition() {
	return this->position;
}

glm::vec3 SceneCamera::getUpVector() {
  return this->upVector;
}

glm::vec3 SceneCamera::getW() {
  return this->w;
}

glm::vec3 SceneCamera::getU() {
	return glm::normalize(glm::cross(getUpVector(), getW()));
}

glm::vec3 SceneCamera::getV() {
	return glm::normalize(glm::cross(getW(), getU()));
}