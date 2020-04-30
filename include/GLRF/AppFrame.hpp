#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

#include <GLRF/Shader.hpp>
#include <GLRF/Scene.hpp>

namespace GLRF {
    class Mouse;
    class AppFrame;
    class App;
}

class GLRF::Mouse
{
private:
    glm::vec2 pos;
    glm::vec2 pos_old;
public:
    Mouse();
    Mouse(double x, double y);
    ~Mouse();

    glm::vec2 getPosition();
    void setPosition(double x, double y);
    glm::vec2 getOffset();
};

class GLRF::AppFrame
{
private:
    ScreenResolution resolution;
    GLFWwindow * window;
    static Mouse mouse;
    App * app;

    static void framebufferSizeCallback(GLFWwindow * window, int width, int height);
    void processInput(GLFWwindow * window);
    static void mouse_callback(GLFWwindow * window, double x, double y);
public:
    AppFrame(ScreenResolution resolution, App * app);
    ~AppFrame();

    bool render();
};

class GLRF::App
{
private:
protected:
    Scene * activeScene;
public:
    virtual ~App() {};
    virtual void configure(GLFWwindow * window) = 0;
    virtual void processUserInput(GLFWwindow * window, glm::vec2 mouse_offset) = 0;
    virtual void updateScene() = 0;
    virtual void render() = 0;
    virtual void setActiveScene(Scene * scene) {
        this->activeScene = scene;
    }
    virtual void forwardUserInputToScene(GLFWwindow * window, glm::vec2 mouse_offset) {
        this->activeScene->processMouse(mouse_offset.x, mouse_offset.y);
        this->activeScene->processInput(window);
    }
};