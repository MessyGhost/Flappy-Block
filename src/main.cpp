#include <memory>
#include "FlappyBlock.h"
#include <glfw/glfw3.h>

std::unique_ptr<FlappyBlock> pgame;

void OnKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                pgame->onEsc();
                break;
            case GLFW_KEY_SPACE:
                pgame->playerJump();
                break;
            case GLFW_KEY_R:
                if(pgame->isPaused()) {
                    pgame->reset(); //not that reset
                }
                break;
        }
    }
}

void OnResize(GLFWwindow *window, int w, int h) {
    pgame->resizeFrame(w, h);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "Flappy Block", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetKeyCallback(window, &OnKey);
    glfwSetFramebufferSizeCallback(window, &OnResize);

    pgame.reset(new FlappyBlock);
    pgame->resizeFrame(800, 600);

    while(glfwWindowShouldClose(window) != GL_TRUE) {
        glfwPollEvents();
        pgame->tick();
        pgame->render();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
