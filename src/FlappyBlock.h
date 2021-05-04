#ifndef FLAPPY_BLOCK_FLAPPYBLOCK_H
#define FLAPPY_BLOCK_FLAPPYBLOCK_H
#include "OpenGL.h"

#include <queue>
#include <chrono>

class FlappyBlock {
public:
    FlappyBlock();
    void tick();
    void render();
    void playerJump();
    void onEsc();
    void resizeFrame(int w, int h);
private:
    void genWall();
private:
    bool playerJumpFlag;
    bool escFlag;

    std::queue<float> wallHeights;

    //gravitational acceleration
    static constexpr float gravity = -9.8f;
    //the speed on y axis to set when jumping
    static constexpr float jumpStrength = 6.0f;

    float playerSpeedX, playerSpeedY, playerHeight;

    VertexBuffer vertexBuffer;
    VertexArray vertexArray;
    ShaderProgram shaderProgram;

    bool firstTick;
    std::chrono::time_point<std::chrono::steady_clock> lastTick;

};


#endif //FLAPPY_BLOCK_FLAPPYBLOCK_H
