#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VectorMath.h"

class SceneCamera {
public:
	SceneCamera(glm::vec3 position, glm::vec3 upVector, glm::vec3 target);
	void rotate(float yaw_offset, float pitch_offset, float sensitivity);
	void rotate(float yaw_offset, float pitch_offset);
	void translate(glm::vec3 direction, float sensitivity);
	void translate(glm::vec3 direction);
	glm::mat4 getViewMatrix();
	glm::vec3 getPosition();
	glm::vec3 UpVector();
	glm::vec3 getW();
	glm::vec3 getU();
	glm::vec3 getV();
private:
	glm::vec3 position, upVector, w;
	float pitch = 0.0f, yaw = 0.0f;
	float sensitivity_rotation = 0.05f;
	float sensitivity_translation = 0.1f;
};