#include <GLRF/AppFrame.hpp>

using namespace GLRF;

Mouse::Mouse():pos(glm::vec2(0, 0)), pos_old(pos) {
    // Mouse(0.0, 0.0);
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
    this->pos_old = this->pos;
    this->pos = glm::vec2(x, y);
}

glm::vec2 Mouse::getOffset() {
    return glm::vec2(this->pos  - this->pos_old);
}

AppFrame::AppFrame(ScreenResolution resolution, App * app) {
    this->resolution = resolution;
    Mouse::getInstance().setPosition(static_cast<double>(resolution.width) / 2.0, static_cast<double>(resolution.height) / 2.0);
    this->window = glfwCreateWindow(resolution.width, resolution.height, "OpenGL", NULL, NULL);
    if (!this->window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD!");
    }
    glfwSetFramebufferSizeCallback(this->window, &AppFrame::framebufferSizeCallback);

    this->app = app;
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
    Mouse::getInstance().setPosition(x, y);
}

bool AppFrame::render() {
    this->app->configure(this->window);
    while(!glfwWindowShouldClose(this->window)) {
        glfwSetCursorPosCallback(this->window, mouse_callback);
        this->app->processUserInput(this->window, Mouse::getInstance().getOffset());
        this->app->updateScene();
        this->app->render();
        glfwSwapBuffers(this->window);
    }
    glfwTerminate();
    return 0;
}