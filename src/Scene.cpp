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

std::shared_ptr<SceneNode<PointLight>> Scene::addObject(std::shared_ptr<PointLight> light) {
	std::shared_ptr<SceneNode<PointLight>> node(new SceneNode<PointLight>(light));
	this->pointLights.push_back(node);
	return node;
}

std::shared_ptr<SceneNode<DirectionalLight>> Scene::addObject(std::shared_ptr<DirectionalLight> light) {
	std::shared_ptr<SceneNode<DirectionalLight>> node(new SceneNode<DirectionalLight>(light));
	this->directionalLights.push_back(node);
	return node;
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

void Scene::draw(ShaderConfiguration * configuration, std::map<GLuint, FrameBuffer*> & map_shader_fbs) {
	ShaderManager & shader_manager = ShaderManager::getInstance();
	shader_manager.clearDrawConfigurations();

	glm::mat4 view = this->activeCamera->getViewMatrix();
	configuration->setMat4("view", view);
	configuration->setVec3("camera_position", this->activeCamera->getPosition());
	configuration->setVec3("camera_view_dir", - this->activeCamera->getW());

	for (unsigned int i = 0; i < this->pointLights.size(); i++) {
		configuration->setVec3("pointLight_position[" + std::to_string(i) + "]", pointLights[i]->getPosition());
		configuration->setVec3("pointLight_color[" + std::to_string(i) + "]", this->pointLights[i]->getObject()->getColor());
		configuration->setFloat("pointLight_power[" + std::to_string(i) + "]", this->pointLights[i]->getObject()->getPower());
	}
	configuration->setUInt("pointLight_count", static_cast<unsigned int>(this->pointLights.size()));

	if (this->directionalLights.size() > 0) {
		glm::vec3 light_dir = glm::vec3(this->directionalLights[0]->calculateModelMatrix()
			* glm::vec4(this->directionalLights[0]->getObject()->getDirection(), 0.f));
		configuration->setVec3("directionalLight_direction", light_dir);
		configuration->setFloat("directionalLight_power", this->directionalLights[0]->getObject()->getPower());
		configuration->setBool("useDirectionalLight", true);
	} else {
		configuration->setBool("useDirectionalLight", false);
	}

	for (unsigned int i = 0; i < this->objectNodes.size(); i++) {
		auto obj = this->objectNodes[i]->getObject();
		GLuint shader_id = obj->getShaderID();
		auto it = map_shader_fbs.find(shader_id);
		if (it == map_shader_fbs.end()) continue;

		auto fb = it->second;
		fb->use();

		// load object-specific values into the internal shader
		ShaderConfiguration object_configuration;
		glm::mat4 modelMat = this->objectNodes[i]->calculateModelMatrix();
		glm::mat3 modelNormalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
		object_configuration.setMat4("model", modelMat);
		object_configuration.setMat3("model_normal", modelNormalMat);
		object_configuration.setMaterial("material", obj->getMaterial());
		
		obj->draw(configuration, &object_configuration);
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
