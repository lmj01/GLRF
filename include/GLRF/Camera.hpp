#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLRF/VectorMath.hpp>

namespace GLRF {
	class Camera;
}

/**
 * @brief A camera that presents its view of a scene to the user when active and rendering.
 * 
 */
class GLRF::Camera {
public:
	/**
	 * @brief Construct a new Camera object.
	 * 
	 * @param position the position of the camera
	 * @param upVector the constant vector that points upwards
	 * @param target a given position that the camera will be looking at
	 */
	Camera(glm::vec3 position, glm::vec3 upVector, glm::vec3 target);

	/**
	 * @brief Rotates the camera by a given angle offset.
	 * 
	 * @param yaw_offset the angle offset (in degrees) around the local y-axis
	 * @param pitch_offset the angle offset (in degrees) around the local x-axis
	 * @param sensitivity the factor that will amplify the offsets
	 */
	void rotate(float yaw_offset, float pitch_offset, float sensitivity = 1.f);

	/**
	 * @brief Moves the camera by a given relative direction.
	 * 
	 * @param direction the relative direction to move to
	 * @param sensitivity the factor that will amplify the offset
	 */
	void translate(glm::vec3 direction, float sensitivity = 1.f);

	/**
	 * @brief Returns the view matrix.
	 * 
	 * @return glm::mat4 the view matrix
	 */
	glm::mat4 getViewMatrix();

	/**
	 * @brief Returns the position.
	 * 
	 * @return glm::mat4 the position
	 */
	glm::vec3 getPosition();

	/**
	 * @brief Returns the constant vector that poinst upwards.
	 * 
	 * @return glm::mat4 the constant vector that poinst upwards
	 */
	glm::vec3 getUpVector();

	/**
	 * @brief Returns the normalized vector that points in the opposite direction of where the camera is looking.
	 * 
	 * @return glm::mat4 the normalized vector that points in the opposite direction of where the camera is looking
	 */
	glm::vec3 getW();

	/**
	 * @brief Returns the camera-relative direction pointing right
	 * 
	 * @return glm::vec3 the camera-relative right pointing right
	 */
	glm::vec3 getU();

	/**
	 * @brief Returns the camera-relative direction pointing upwards
	 * 
	 * @return glm::vec3 the camera-relative direction pointing upwards
	 */
	glm::vec3 getV();
private:
	glm::vec3 position, upVector, w;
	float pitch = 0.f;
	float yaw = 0.f;
	const float sensitivity_factor_rotation = 0.05f;
	const float sensitivity_factor_translation = 0.1f;
	float sensitivity_rotation;
	float sensitivity_translation;
};