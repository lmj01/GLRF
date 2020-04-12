#include <GLRF/Scene.hpp>

Scene::Scene(std::shared_ptr<SceneCamera> camera) {
	addObject(camera);
	setActiveCamera(camera);
}

Scene::Scene() {
	setActiveCamera(std::shared_ptr<SceneCamera>(
		new SceneCamera(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)))
	);
}

void Scene::addObject(std::shared_ptr<SceneMesh> mesh, glm::vec3 position, glm::mat4 rotation) {
	SceneMeshNode node = SceneMeshNode(mesh);
	node.setPositionAndRotation(position, rotation);
	this->meshNodes.push_back(node);
}

void Scene::addObject(std::shared_ptr<PointLight> light) {
	if (std::find(this->pointLights.begin(), this->pointLights.end(), light) != this->pointLights.end()) {
		this->pointLights.push_back(light);
	}
}

void Scene::addObject(std::shared_ptr<DirectionalLight> light) {
	this->directionalLight = light;
}

void Scene::addObject(std::shared_ptr<SceneCamera> camera) {
	this->cameras.push_back(camera);
}

void Scene::setActiveCamera(std::shared_ptr<SceneCamera> camera) {
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
		//glm::vec4 P = view * glm::vec4(pointLights[i]->getPosition(), 1.f);
		shader.setValue("pointLight_position[" + std::to_string(i) + "]", pointLights[i]->getPosition());
		shader.setValue("pointLight_color[" + std::to_string(i) + "]", this->pointLights[i]->getColor());
		shader.setValue("pointLight_power[" + std::to_string(i) + "]", this->pointLights[i]->getPower());
	}
	shader.setValue("pointLight_count", (int)this->pointLights.size());

	if (this->directionalLight) {
		//glm::vec3 dir = view * glm::vec4(this->directionalLight->getDirection(), 0.f);
		//std::cout << "(" << dir.x << ", " << dir.y << ", " << dir.z << ")" << std::endl;
		//glm::vec3 dir = view * glm::vec4(0.0, -1.0, 0.0, 0.f);
		shader.setValue("directionalLight_direction", this->directionalLight->getDirection());
		shader.setValue("directionalLight_power", this->directionalLight->getPower());
		shader.setValue("useDirectionalLight", true);
	} else {
		shader.setValue("useDirectionalLight", false);
	}

	for (unsigned int i = 0; i < this->meshNodes.size(); i++) {
		glm::mat4 modelMat = this->meshNodes[i].getModelMatrix();
		glm::mat3 modelNormalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
		shader.setValue("model", modelMat);
		shader.setValue("model_normal", modelNormalMat);
		shader.setMaterial("material", this->meshNodes[i].getMesh()->getMaterial());

		this->meshNodes[i].getMesh()->draw();
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
