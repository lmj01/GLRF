#include "SceneCamera.h"

SceneCamera::SceneCamera(glm::vec3 position, glm::vec3 upVector, glm::vec3 target) {
	this->position = position;
	this->upVector = glm::normalize(upVector);
	this->w = glm::normalize(position - target);
	float cos_pitch = glm::dot(this->upVector, getV());
	this->pitch = -glm::degrees(acos(cos_pitch));
}

void SceneCamera::rotate(float yaw_offset, float pitch_offset, float sensitivity) {
	float modified_yaw_offset = yaw_offset * sensitivity;
	float modified_pitch_offset = pitch_offset * sensitivity;
	float pitch_current = this->pitch + modified_pitch_offset;
	if (pitch_current > 85.0 || pitch_current < -85.0) modified_pitch_offset = 0.0f;
	float cos_pitch = cos(glm::radians(-modified_pitch_offset));
	float sin_pitch = sin(glm::radians(-modified_pitch_offset));

	glm::mat3 rotationCamToWorld = glm::mat3(w, getV(), getU());
	glm::vec3 direction = glm::vec3(0.0f);
	direction.x = cos_pitch * cos(glm::radians(-modified_yaw_offset));
	direction.y = sin_pitch;
	direction.z = cos_pitch * sin(glm::radians(-modified_yaw_offset));
	this->w = glm::normalize(rotationCamToWorld * direction);
	this->yaw += modified_yaw_offset;
	this->pitch += modified_pitch_offset;
}

void SceneCamera::rotate(float yaw_offset, float pitch_offset) {
	rotate(yaw_offset, pitch_offset, this->sensitivity_rotation);
}

void SceneCamera::translate(glm::vec3 direction, float sensitivity) {
	this->position += direction * sensitivity;
}

void SceneCamera::translate(glm::vec3 direction) {
	translate(direction, this->sensitivity_translation);
}

glm::mat4 SceneCamera::getViewMatrix() {
	return glm::lookAt(this->position, this->position - this->w, this->upVector);
}

glm::vec3 SceneCamera::getPosition() {
	return this->position;
}

glm::vec3 SceneCamera::UpVector() {
  return this->upVector;
}

glm::vec3 SceneCamera::getW() {
  return this->w;
}

glm::vec3 SceneCamera::getU() {
	return glm::cross(this->upVector, this->w);
}

glm::vec3 SceneCamera::getV() {
	return glm::cross(this->w, getU());
}