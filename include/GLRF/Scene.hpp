#pragma once
#include <vector>
#include <algorithm>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLRF/Shader.hpp>
#include <GLRF/SceneCamera.hpp>
#include <GLRF/SceneObject.hpp>
#include <GLRF/SceneLight.hpp>
#include <GLRF/VectorMath.hpp>

class Scene {
public:
	Scene(std::shared_ptr<SceneCamera> camera);
	Scene();
	void addObject(std::shared_ptr<SceneMesh> mesh, glm::vec3 position, glm::mat4 rotation);
	void addObject(std::shared_ptr<PointLight> light);
	void addObject(std::shared_ptr<DirectionalLight> light);
	void addObject(std::shared_ptr<SceneCamera> camera);
	void setActiveCamera(std::shared_ptr<SceneCamera> camera);
	void draw(Shader & shader);
	void processInput(GLFWwindow * window);
	void processMouse(float xOffset, float yOffset);
private:
	std::vector<SceneMeshNode> meshNodes;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::shared_ptr<DirectionalLight> directionalLight = nullptr;
	std::vector<std::shared_ptr<SceneCamera>> cameras;
	std::shared_ptr<SceneCamera> activeCamera;
};