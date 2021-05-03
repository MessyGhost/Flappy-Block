#ifndef FLAPPY_BLOCK_FLAPPYBLOCK_H
#define FLAPPY_BLOCK_FLAPPYBLOCK_H
#include "OpenGL.h"

#include <queue>

class FlappyBlock {
public:
    FlappyBlock();
    void tick();
    void render();
    void playerJump();
    void onEsc();
private:
    void genWall();
private:
    bool playerJumpFlag;
    bool escFlag;

    std::queue<float> wallHeights;

    static constexpr float gravity = -1.0f;
    float playerSpeedX, playerSpeedY;
};


#endif //FLAPPY_BLOCK_FLAPPYBLOCK_H
