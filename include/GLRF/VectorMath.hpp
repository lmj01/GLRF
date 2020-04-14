#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <GLRF/VertexFormat.hpp>

/**
 * @brief Calculates the tangent vector by using 3 points.
 * 
 * @param p1 the points that the tangent vector will be created for
 * @param p2 the first neighbor point
 * @param p3 the second neighbor point
 * @return glm::vec3 the tangent vector
 */
glm::vec3 calculateTangent(VertexFormat p1, VertexFormat p2, VertexFormat p3);

/**
 * @brief Calculates and sets the tangent vectors for multiple GL_TRIANGLES.
 * 
 * @param vertices the vertices of the triangles (always a multiple of 3)
 */
void calculateAndSetTangents_GL_TRIANGLES(std::vector<VertexFormat> * vertices);

/**
 * @brief Calculates and sets the tangent vectors for multiple verticles with a specified OpenGL draw type.
 * 
 * @param vertices multiple vertices of the given OpenGL draw type
 * @param drawType the OpenGL draw type that defines the connections of the vertices
 */
void calculateAndSetTangents(std::vector<VertexFormat> * vertices, GLenum drawType);

/**
 * @brief Dehomogenizes the 4d vector and converts it to a 3d vector.
 * 
 * @param homogeneous_input the input vector
 * @return glm::vec3 the dehomogenized vector
 */
glm::vec3 dehomogenizeVec4(glm::vec4 homogeneous_input);

/**
 * @brief Generate a random normalized vector.
 * 
 * @return glm::vec3 a normalized vector
 */
glm::vec3 generateRandomNormalizedVector();

/**
 * @brief Finds a randomly oriented vector that is orthonormal to a reference vector.
 * 
 * @param v the reference vector that the new vector will be orthonormal to
 * @return glm::vec3 a vector that is orthonormal to the specified reference vector.
 */
glm::vec3 findNonLinearNormalizedVector(glm::vec3 v);

/**
 * @brief Generates a random float value between 0 and 1
 * 
 * @return float the randomly generated value
 */
float generateRandomFloat();