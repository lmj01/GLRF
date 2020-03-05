#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "VertexFormat.h"

glm::vec3 calculateTangent(VertexFormat p1, VertexFormat p2, VertexFormat p3);
void calculateAndSetTangents_GL_TRIANGLES(std::vector<VertexFormat> * vertices);
void calculateAndSetTangents(std::vector<VertexFormat> * vertices, GLenum drawType);
glm::vec3 dehomogenizeVec4(glm::vec4 homogeneous_input);
glm::vec3 getRandomNormalizedVector();
glm::vec3 findNonLinearNormalizedVector(glm::vec3 v);
float getRandomFloat();