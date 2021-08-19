#include <GL3/gl3.h>
#include <GL3/gl3w.h>
#include <GLFW/glfw3.h>

#include "Scene.hpp"

GLFWwindow *initializeOpenGLContext()
{
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "ClosedGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    return window;
}

int main(int argc, char **argv)
{
    GLFWwindow *mainWindow = initializeOpenGLContext();
    Scene *mainScene = new Scene(mainWindow);

    while (!glfwWindowShouldClose(mainWindow))
        mainScene->AdvanceFrame();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
