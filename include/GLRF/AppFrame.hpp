#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

#include <GLRF/Shader.hpp>

namespace GLRF {
    class Mouse;
    class AppFrame;
}

class GLRF::Mouse {
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
    double xpos_prev = 0.0;
    double ypos_prev = 0.0;
    GLFWwindow * window;
    Mouse mouse;
public:
    AppFrame(ScreenResolution resolution);
    ~AppFrame();

    static void framebufferSizeCallback(GLFWwindow * window, int width, int height);
    void processInput(GLFWwindow * window);
    void mouse_callback(GLFWwindow * window, double x, double y);
};