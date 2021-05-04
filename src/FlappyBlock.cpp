#include "FlappyBlock.h"
#include <random>
#include <chrono>

FlappyBlock::FlappyBlock()
    :playerJumpFlag(false), escFlag(false), vertexBuffer(GL_DYNAMIC_DRAW, 6 * 2 * sizeof(float)),
    shaderProgram(ShaderProgram::loadFromFile("resource/shaders/rect.vert", "resource/shaders/rect.frag"))
{
    vertexBuffer.map(GL_WRITE_ONLY, [&](void *data) {
       float vertices[] = {
          -0.5f, -0.5f,
          0.5f, -0.5f,
          0.5f, 0.5f,  //right bottom
          -0.5f, -0.5f,
          0.5f, 0.5f,
          -0.5f, 0.5f //left top
       } ;

        memcpy(data, vertices, sizeof(vertices));
    });

    vertexBuffer.bind(GL_ARRAY_BUFFER);
    vertexArray.setAttribute(0, 2, GL_FLOAT, false, 0, 0);
    vertexArray.enableAttribute(0);
}

void FlappyBlock::tick() {
    if(playerJumpFlag) {
        playerJumpFlag = false;
    }

    if(escFlag) {
        escFlag = false;
    }
}

void FlappyBlock::render() {
    glClearColor(0.5f, 1.0f, 0.5f, 1.0f); //(very) light green
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.use();
    vertexArray.drawArrays(GL_TRIANGLES, 0, 6);
}

void FlappyBlock::playerJump() {
    playerJumpFlag = true;
}

void FlappyBlock::onEsc() {
    escFlag = false;
}

void FlappyBlock::genWall() {
    static std::default_random_engine e;
    e.seed(std::time(nullptr));
    static std::uniform_real_distribution u(0.0f, 1.0f);
    wallHeights.push(u(e));
}
