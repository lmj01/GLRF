#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VectorMath.h"

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

class DirectionalLight {
public:
	DirectionalLight(float xDegree, float yDegree, float power);
	void rotate(float x, float y);
	glm::vec3 getDirection();
	float getPower();
private:
	float power = 1.0f;
	float xDegreeTotal = 0.0f;
	float yDegreeTotal = 0.0f;
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
};