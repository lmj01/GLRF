#include "Scene.hpp"

Scene::Scene(SceneCamera * camera) {
	addObject(camera);
	setActiveCamera(camera);
}

Scene::Scene() {
	this->activeCamera = &SceneCamera(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
}

void Scene::addObject(SceneMesh * mesh, glm::vec3 position, glm::mat4 rotation) {
	SceneMeshNode node = SceneMeshNode(mesh);
	node.setPositionAndRotation(position, rotation);
	this->meshNodes.push_back(node);
}

void Scene::addObject(PointLight * light) {
	this->pointLights.push_back(light);
}

void Scene::addObject(DirectionalLight * light) {
	this->directionalLight = light;
}

void Scene::addObject(SceneCamera * camera) {
	this->cameras.push_back(camera);
}

void Scene::setActiveCamera(SceneCamera * camera) {
	this->activeCamera = camera;
}

void Scene::draw(Shader shader) {
	glm::mat4 view = this->activeCamera->getViewMatrix();
	shader.setMat4("view", view);
	shader.setVec3("camera_position", glm::vec4(this->activeCamera->getPosition(), 1.0));

	for (unsigned int i = 0; i < this->pointLights.size(); i++) {
		//glm::vec4 P = view * glm::vec4(pointLights[i]->getPosition(), 1.f);
		shader.setVec3("pointLight_position[" + std::to_string(i) + "]", pointLights[i]->getPosition());
		shader.setVec3("pointLight_color[" + std::to_string(i) + "]", this->pointLights[i]->getColor());
		shader.setFloat("pointLight_power[" + std::to_string(i) + "]", this->pointLights[i]->getPower());
	}
	shader.setUInt("pointLight_count", this->pointLights.size());

	if (this->directionalLight) {
		//glm::vec3 dir = view * glm::vec4(this->directionalLight->getDirection(), 0.f);
		//std::cout << "(" << dir.x << ", " << dir.y << ", " << dir.z << ")" << std::endl;
		//glm::vec3 dir = view * glm::vec4(0.0, -1.0, 0.0, 0.f);
		shader.setVec3("directionalLight_direction", this->directionalLight->getDirection());
		shader.setFloat("directionalLight_power", this->directionalLight->getPower());
		shader.setBool("useDirectionalLight", true);
	} else {
		shader.setBool("useDirectionalLight", false);
	}

	for (unsigned int i = 0; i < this->meshNodes.size(); i++) {
		glm::mat4 modelMat = this->meshNodes[i].getModelMatrix();
		glm::mat3 modelNormalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
		shader.setMat4("model", modelMat);
		shader.setMat3("model_normal", modelNormalMat);
		shader.setMaterial("material", this->meshNodes[i].getMesh()->getMaterial());

		this->meshNodes[i].getMesh()->draw();
	}
}

void Scene::processInput(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		activeCamera->translate(- activeCamera->getU());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		activeCamera->translate(activeCamera->getU());
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		activeCamera->translate(- activeCamera->getW());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		activeCamera->translate(activeCamera->getW());
}

void Scene::processMouse(float xOffset, float yOffset) {
	this->activeCamera->rotate(xOffset, yOffset);
}
