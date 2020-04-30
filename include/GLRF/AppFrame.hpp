#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

#include <GLRF/Shader.hpp>

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
    Mouse mouse;
    App& app;

    static void framebufferSizeCallback(GLFWwindow * window, int width, int height);
    void processInput(GLFWwindow * window);
    void mouse_callback(GLFWwindow * window, double x, double y);
public:
    AppFrame(ScreenResolution resolution, App& app);
    ~AppFrame();

    bool render();
};

class GLRF::App
{
private:
public:
    virtual void configure(GLFWwindow * window);
    virtual void processUserInput(GLFWwindow * window, glm::vec2 mouse_offset);
    virtual void updateScene();
    virtual void render();
};