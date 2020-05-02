#pragma once
#include <vector>
#include <algorithm>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLRF/Shader.hpp>
#include <GLRF/Camera.hpp>
#include <GLRF/SceneObject.hpp>
#include <GLRF/SceneLight.hpp>
#include <GLRF/VectorMath.hpp>

namespace GLRF {
	class Scene;
}

/**
 * @brief A 3d space can contain objects, lights and cameras.
 * 
 */
class GLRF::Scene {
public:
	/**
	 * @brief Construct a new Scene object.
	 * 
	 * @param camera the new active camera
	 */
	Scene(std::shared_ptr<Camera> camera);

	/**
	 * @brief Construct a new Scene object.
	 * 
	 * A default camera will be created and set as active.
	 */
	Scene();

	/**
	 * @brief Adds an object to the scene.
	 * 
	 * @param object the object that will be added to the scene
	 */
	template <class T>
	std::shared_ptr<SceneNode<SceneObject>> addObject(std::shared_ptr<T> object) {
		static_assert(std::is_base_of<SceneObject, T>::value, "T must extend SceneObject");
		std::shared_ptr<SceneNode<SceneObject>> node(new SceneNode<SceneObject>(object));
		this->objectNodes.push_back(node);
		return node;
	}

	/**
	 * @brief Adds a point lightsource to the scene.
	 * 
	 * @param light the point lightsource that will be added to the scene
	 */
	std::shared_ptr<SceneNode<PointLight>> addObject(std::shared_ptr<PointLight> light);

	/**
	 * @brief Adds a directional lightsource to the scene.
	 * 
	 * @param light the directional lightsource that will be added to the scene
	 */
	std::shared_ptr<SceneNode<DirectionalLight>> addObject(std::shared_ptr<DirectionalLight> light);

	/**
	 * @brief Adds a camera to the scene. The camera will not become active.
	 * 
	 * @param camera the camera the will be added to the scene
	 */
	void addObject(std::shared_ptr<Camera> camera);

	/**
	 * @brief Sets the given camera as the active camera.
	 * 
	 * @param camera the camera that will be added to the scene
	 * 
	 * If the camera is new, it will also be added to the scene.
	 */
	void setActiveCamera(std::shared_ptr<Camera> camera);

	/**
	 * @brief Draws all objects of the scene with the given shader.
	 * 
	 * @param shader the shader to draw the scenes objects with
	 */
	void draw(Shader & shader);

	/**
	 * @brief Processes keyboard inputs for the scene.
	 * 
	 * @param window the window that receives the current inputs
	 */
	void processInput(GLFWwindow * window);

	/**
	 * @brief Processes mouse inputs for the scene.
	 * 
	 * @param xOffset the offset of the mouse position in pixels along the x-axis of the mouse since the last measurement
	 * @param yOffset the offset of the mouse position in pixels along the y-axis of the mouse since the last measurement
	 */
	void processMouse(float xOffset, float yOffset);
private:
	std::vector<std::shared_ptr<SceneNode<SceneObject>>> objectNodes;
	std::vector<std::shared_ptr<SceneNode<PointLight>>> pointLights;
	std::vector<std::shared_ptr<SceneNode<DirectionalLight>>> directionalLights;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activeCamera;
};