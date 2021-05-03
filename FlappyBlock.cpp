#include "FlappyBlock.h"
#include <random>
#include <chrono>

FlappyBlock::FlappyBlock()
    :playerJumpFlag(false), escFlag(false)
{

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
