#include <GLRF/Camera.hpp>

using namespace GLRF;

const float Camera::LIMIT_EPSILON = 0.0001f;
const float Camera::SENSITIVITY_ROTATION = 0.001f;
const float Camera::SENSITIVITY_TRANSLATION = 0.1f;
const float Camera::LIMIT_PITCH_MAX = M_PI_2;

Camera::Camera(glm::vec3 position, glm::vec3 up_vector, glm::vec3 target)
{
	this->position = position;
	this->up_vector = glm::normalize(up_vector);
	this->w = glm::normalize(position - target);
	setPitchLimit(-1.f);
	setYawLimit(-1.f);
	setPitch(glm::asin(glm::dot(getW(), this->up_vector)));
	setYaw(0.f);
	this->ref_x = glm::normalize(getW() - glm::dot(getW(), this->up_vector) * (this->up_vector));
	this->ref_z = getU();
}

void Camera::rotate(float yaw_offset, float pitch_offset, float sensitivity)
{
	yaw_offset *= sensitivity * SENSITIVITY_ROTATION * sensitivity_rotation_user;
	pitch_offset *= sensitivity * SENSITIVITY_ROTATION * sensitivity_rotation_user;
	setPitch(this->pitch - pitch_offset);
	setYaw(this->yaw - yaw_offset);
	buildW();
}

void Camera::translate(glm::vec3 direction, float sensitivity)
{
	this->position += direction * sensitivity * SENSITIVITY_TRANSLATION * sensitivity_translation_user;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->position - this->w, this->up_vector);
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::vec3 Camera::getUpVector()
{
  return this->up_vector;
}

glm::vec3 Camera::getW()
{
  return this->w;
}

glm::vec3 Camera::getU()
{
	return glm::normalize(glm::cross(getUpVector(), getW()));
}

glm::vec3 Camera::getV()
{
	return glm::normalize(glm::cross(getW(), getU()));
}

float Camera::setPitch(float pitch)
{
	float tmp = this->pitch;
	this->pitch = glm::clamp(pitch, -this->pitch_limit, this->pitch_limit);
	return tmp;
}

float Camera::setYaw(float yaw)
{
	float tmp = this->yaw;
	this->yaw = (this->yaw_limit < 0.f) ? yaw : glm::clamp(yaw, -this->yaw_limit, this->yaw_limit);
	return tmp;
}

void Camera::buildW()
{
	float cos_pitch = glm::cos(this->pitch);
	float sin_pitch = glm::sin(this->pitch);

	this->w = cos_pitch * glm::cos(this->yaw) * this->ref_x
		+ sin_pitch * this->up_vector
		+ cos_pitch * glm::sin(this->yaw) * this->ref_z;
}

void Camera::setPitchLimit(float limit)
{
	this->pitch_limit = (limit < 0.f ? LIMIT_PITCH_MAX : glm::min(limit, LIMIT_PITCH_MAX)) - LIMIT_EPSILON;
}

void Camera::setYawLimit(float limit)
{
	this->yaw_limit = limit - LIMIT_EPSILON;
}

void Camera::setSensitivityForRotation(float sensitivity) {
	this->sensitivity_rotation_user = sensitivity;
}

void Camera::setSensitivityForTranslation(float sensitivity) {
	this->sensitivity_translation_user = sensitivity;
}