#ifndef FLAPPY_BLOCK_FLAPPYBLOCK_H
#define FLAPPY_BLOCK_FLAPPYBLOCK_H
#include "OpenGL.h"
#include "BlockyVideoMemoryPool.h"

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
private: //structures
    struct Wall {
        Wall(double distance, float height) noexcept
            :distance(distance), height(height)
        {

        }
        double distance;
        float height;
        GLuint memOffset;
    };
private: //funcs
    void genWall();
    bool isWallDistanceVisible(double distance) const noexcept;
private: //data
    //events
    bool playerJumpFlag;
    bool escFlag;
    bool playerWasHit;
    bool firstTick;
    std::chrono::time_point<std::chrono::steady_clock> lastTick;

    //game data
    std::deque<Wall> walls;
    float playerSpeedX, playerSpeedY, playerHeight;
    double distance;

    //other data
    float halfWidth;

    //constants
    //gravitational acceleration
    static constexpr float gravity = -9.8f;
    //the speed on y axis to set when jumping
    static constexpr float jumpStrength = 6.0f;
    static constexpr float wallSpacing = 8.0f;
    static constexpr float wallWidth = 1.5f;
    static constexpr float wallGap = jumpStrength * 3.0f / 4.0f;
    static constexpr float wallPadding = 1.0f;
    static constexpr float frameHeight = 16.0f;

    //render
    VertexBuffer playerVB;
    VertexArray playerVA;
    BlockyVideoMemoryPool wallsVB;
    VertexArray wallsVA;
    static constexpr GLsizei singleWallVideoDataSize = 6 * 2 * 2 * sizeof(float); //6 triangles * (top + bottom = 2) * sizeof(2 * float)
    ShaderProgram shaderProgram;
};


#endif //FLAPPY_BLOCK_FLAPPYBLOCK_H