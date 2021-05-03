#include <memory>
#include "FlappyBlock.h"
#include <glfw/glfw3.h>

std::unique_ptr<FlappyBlock> pgame;

void OnKey(GLFWwindow *window, int key, int scancode, int action, int mod) {
    if(action == GLFW_KEY_DOWN) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
                pgame->onEsc();
                break;
            case GLFW_KEY_SPACE:
                pgame->playerJump();
                break;
        }
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "Flappy Block", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, &OnKey);

    pgame.reset(new FlappyBlock);

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
