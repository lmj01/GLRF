#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLRF/VectorMath.hpp>
#include <GLRF/MathUtil.hpp>

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

	/**
	 * @brief Sets the limit for the pitch of the camera.
	 * Negative values default the limit to pi/2.
	 * Values get clamped at pi/2.
	 * 
	 * @param limit the angle in radians
	 */
	void setPitchLimit(float limit);

	/**
	 * @brief Sets the limit for the yaw of the camera.
	 * Negative values deactivate the limit.
	 *
	 * @param limit the angle in radians
	 */
	void setYawLimit(float limit);
private:
	glm::vec3 position, up_vector, w, ref_x, ref_z;
	float pitch = 0.f;
	float yaw = 0.f;
	float pitch_limit;
	float yaw_limit;
	static const float LIMIT_EPSILON;
	static const float LIMIT_PITCH_MAX;
	static const float SENSITIVITY_ROTATION;
	static const float SENSITIVITY_TRANSLATION;

	/**
	 * @brief Sets the pitch of the camera to the given value and returns the previous pitch angle.
	 * If a limit has been defined, clamp to limit range.
	 *
	 * @param pitch the pitch angle in radians
	 * @return previous_pitch the previous value of the pitch angle in radians
	 */
	float setPitch(float pitch);

	/**
	 * @brief Sets the yaw of the camera to the given value and returns the previous yaw angle.
	 * If a limit has been defined, clamp to limit range.
	 *
	 * @param yaw the yaw angle in radians
	 * @return previous_yaw the previous value of the yaw angle in radians
	 */
	float setYaw(float yaw);

	/**
	 * @brief Builds a new w-vector from the current pitch and yaw angles.
	 */
	void buildW();
};