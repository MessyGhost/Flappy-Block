#include "FlappyBlock.h"
#include <random>
#include "RenderUtil.h"


FlappyBlock::FlappyBlock()
    :playerJumpFlag(false), escFlag(false), vertexBuffer(GL_DYNAMIC_DRAW, 6 * 2 * sizeof(float)),
    playerSpeedX(0.0f), playerSpeedY(1.0f), playerHeight(0.0f),
    shaderProgram(ShaderProgram::loadFromFile("resource/shaders/rect.vert", "resource/shaders/rect.frag")),
    firstTick(true)
{
    vertexBuffer.map(GL_WRITE_ONLY, [&](void *data) {
        auto vertices = makeRect(-0.5f, 0.5f, 0.5f, -0.5f);

        memcpy(data, vertices.data(), 6 * 2 * sizeof(float));
    });

    vertexBuffer.bind(GL_ARRAY_BUFFER);
    vertexArray.setAttribute(0, 2, GL_FLOAT, false, 0, 0);
    vertexArray.enableAttribute(0);
}

void FlappyBlock::tick() {
    //init
    if(firstTick) {
        firstTick = false;
        lastTick = std::chrono::steady_clock::now();
    }

    //handle events
    if(playerJumpFlag) {
        playerJumpFlag = false;
        playerSpeedY = jumpStrength;
    }

    if(escFlag) {
        escFlag = false;
    }

    //do logical calculations
    auto deltaSecond = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTick).count() / 1000000.0f;
    playerSpeedY += gravity * deltaSecond;
    playerHeight += playerSpeedY * deltaSecond;

    lastTick = std::chrono::steady_clock::now(); //update last tick
}

#include <glm/gtc/matrix_transform.hpp>

void FlappyBlock::render() {
    glClearColor(0.5f, 1.0f, 0.5f, 1.0f); //(very) light green
    glClear(GL_COLOR_BUFFER_BIT);
    shaderProgram.setUniform("modelView", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, playerHeight, 0.0f)));
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

void FlappyBlock::resizeFrame(int width, int height) {
    float w = width, h = height;
    float ratio = w / h;
    float frameHeight = 16.0f;

    float halfWidth = ratio * frameHeight / 2.0f, halfHeight = frameHeight / 2.0f;
    shaderProgram.setUniform("projection", glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight));
    glViewport(0, 0, width, height);
}
