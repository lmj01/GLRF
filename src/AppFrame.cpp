#include <GLRF/AppFrame.hpp>

using namespace GLRF;

Mouse::Mouse() {
    Mouse(0.0, 0.0);
}

Mouse::Mouse(double x, double y) {
    this->pos = glm::vec2(x, y);
    this->pos_old = this->pos;
}

Mouse::~Mouse() {
    delete &pos_old;
    delete &pos;
}

glm::vec2 Mouse::getPosition() {
    return glm::vec2(this->pos);
}

void Mouse::setPosition(double x, double y) {
    this->pos = glm::vec2(x, y);
}

glm::vec2 Mouse::getOffset() {
    return glm::vec2(this->pos  - this->pos_old);
}

AppFrame::AppFrame(ScreenResolution resolution) {
    this->resolution = resolution;
    this->mouse = Mouse(static_cast<double>(resolution.width) / 2.0, static_cast<double>(resolution.height) / 2.0);
    this->window = glfwCreateWindow(resolution.width, resolution.height, "OpenGL", NULL, NULL);
    if (!this->window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD!");
    }
    glfwSetFramebufferSizeCallback(this->window, &AppFrame::framebufferSizeCallback);
}

AppFrame::~AppFrame() {

}

void AppFrame::framebufferSizeCallback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void AppFrame::processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void AppFrame::mouse_callback(GLFWwindow * window, double x, double y) {
    this->mouse.setPosition(x, y);
}