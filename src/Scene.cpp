#include <GLRF/Scene.hpp>

using namespace GLRF;

Scene::Scene(std::shared_ptr<Camera> camera) {
	addObject(camera);
	setActiveCamera(camera);
}

Scene::Scene() {
	setActiveCamera(std::shared_ptr<Camera>(
		new Camera(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)))
	);
}

void Scene::addObject(SceneNode<SceneMesh> node) {
	this->meshNodes.push_back(node);
}

void Scene::addObject(SceneNode<PointLight> light) {
	this->pointLights.push_back(light);
}

void Scene::addObject(SceneNode<DirectionalLight> light) {
	this->directionalLights.push_back(light);
}

void Scene::addObject(std::shared_ptr<Camera> camera) {
	this->cameras.push_back(camera);
}

void Scene::setActiveCamera(std::shared_ptr<Camera> camera) {
	auto it = std::find(this->cameras.begin(), this->cameras.end(), camera);
	if (it == this->cameras.end()) {
		this->cameras.push_back(camera);
	}
	this->activeCamera = camera;
}

void Scene::draw(Shader & shader) {
	glm::mat4 view = this->activeCamera->getViewMatrix();
	shader.setValue("view", view);
	shader.setValue("camera_position", this->activeCamera->getPosition());

	for (unsigned int i = 0; i < this->pointLights.size(); i++) {
		shader.setValue("pointLight_position[" + std::to_string(i) + "]", pointLights[i].getPosition());
		shader.setValue("pointLight_color[" + std::to_string(i) + "]", this->pointLights[i].getObject()->getColor());
		shader.setValue("pointLight_power[" + std::to_string(i) + "]", this->pointLights[i].getObject()->getPower());
	}
	shader.setValue("pointLight_count", static_cast<unsigned int>(this->pointLights.size()));

	if (this->directionalLights.size() > 0) {
		shader.setValue("directionalLight_direction", this->directionalLights[0].getObject()->getDirection());
		shader.setValue("directionalLight_power", this->directionalLights[0].getObject()->getPower());
		shader.setValue("useDirectionalLight", true);
	} else {
		shader.setValue("useDirectionalLight", false);
	}

	for (unsigned int i = 0; i < this->meshNodes.size(); i++) {
		glm::mat4 modelMat = this->meshNodes[i].calculateModelMatrix();
		glm::mat3 modelNormalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
		shader.setValue("model", modelMat);
		shader.setValue("model_normal", modelNormalMat);
		shader.setMaterial("material", this->meshNodes[i].getObject()->getMaterial());

		this->meshNodes[i].getObject()->draw();
	}
}

void Scene::processInput(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		activeCamera->translate(	-	activeCamera->getU());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		activeCamera->translate(		activeCamera->getU());
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		activeCamera->translate(	-	activeCamera->getW());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		activeCamera->translate(		activeCamera->getW());
}

void Scene::processMouse(float xOffset, float yOffset) {
	this->activeCamera->rotate(xOffset, yOffset);
}
