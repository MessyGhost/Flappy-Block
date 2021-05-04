#include "FlappyBlock.h"
#include <random>
#include "RenderUtil.h"


FlappyBlock::FlappyBlock()
    : playerJumpFlag(false), escFlag(false),
      playerVB(GL_STATIC_DRAW, 6 * 2 * sizeof(float)),
      wallsVB(singleWallVideoDataSize),
      playerSpeedX(2.75f),
      shaderProgram(ShaderProgram::loadFromFile("resource/shaders/rect.vert", "resource/shaders/rect.frag")),
      compositeProgram(ShaderProgram::loadFromFile("resource/shaders/composite.vert", "resource/shaders/composite.frag")),
      firstTick(true), paused(true)
{
    reset();

    //init render
    wallsVB.setOnRealloc([&, this](const VertexBuffer &buffer) {
        buffer.bind(GL_ARRAY_BUFFER);
        wallsVA.setAttribute(0, 2, GL_FLOAT, false, 0, 0);
        buffer.unbind(GL_ARRAY_BUFFER);
    });

    {
        auto &buffer = wallsVB.getInnerBuffer();
        buffer.bind(GL_ARRAY_BUFFER);
        wallsVA.setAttribute(0, 2, GL_FLOAT, false, 0, 0);
        wallsVA.enableAttribute(0);
    }

    playerVB.map(GL_WRITE_ONLY, [&](void *data) {
        auto vertices = makeRect(-0.5f, 0.5f, 0.5f, -0.5f);

        memcpy(data, vertices.data(), 6 * 2 * sizeof(float));
    });

    playerVB.bind(GL_ARRAY_BUFFER);
    playerVA.setAttribute(0, 2, GL_FLOAT, false, 0, 0);
    playerVA.enableAttribute(0);
}

void FlappyBlock::tick() {
    if(escFlag) { //pause
        escFlag = false;
        paused = !paused;
    }

    if(paused) {
        lastTick = std::chrono::steady_clock::now();
        playerJumpFlag = false;
        return;
    }

    //init
    if(firstTick) {
        firstTick = false;
        lastTick = std::chrono::steady_clock::now();
    }

    if(playerJumpFlag) { //jump
        playerJumpFlag = false;
        playerSpeedY = jumpStrength;
    }

    //update walls
    while(walls.size() > 1 && !isWallDistanceVisible(walls.front().distance)) { //clear unvisible walls
        auto &front = walls.front();
        wallsVB.free(front.memOffset);
        walls.pop_front();
    }

    while(walls.empty() || isWallDistanceVisible(walls.back().distance)) { //gen new walls;
        genWall();
    }

    //do logical calculations
    //motion
    auto deltaSecond = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTick).count() / 1000000.0f;
    playerSpeedY += gravity * deltaSecond;
    playerHeight += playerSpeedY * deltaSecond;

    distance += playerSpeedX * deltaSecond;

    //crashing with walls
    for(auto &wall : walls) {
        auto dist = wall.distance - distance + wallWidth;
        if(dist > wallSpacing) { //not begin
            playerWasHit = false;
        }
        else if(dist > 0.0f && dist < 0.5f * wallSpacing) { //closest && front wall
            //aabb box testing
            auto playerBoundLeft = distance - 0.5f, playerBoundRight = playerBoundLeft + 1.0f;
            auto playerBoundTop = playerHeight + 0.5f, playerBoundBottom = playerBoundTop - 1.0f;
            auto gapBoundLeft = wall.distance - wallWidth / 2.0f, gapBoundRight = gapBoundLeft + wallWidth;
            auto gapBoundTop = (wall.height - 0.5f) * (frameHeight - 2 * wallPadding - wallGap / 2.0f) + wallGap / 2.0f,
            gapBoundBottom = gapBoundTop - wallGap;

            auto isSubset = [](float pmin, float pmax, float smin, float smax) -> bool {
                return pmin <= smin && pmax >= smax;
            };

            auto hasIntersection = [&](float s1min, float s1max, float s2min, float s2max) -> bool {
                if(isSubset(s1min, s1max, s2min, s2max) || isSubset(s2min, s2max, s1min, s1max)) { //one is another's subset
                    return true;
                } else {
                    if(s1min < s2min) {
                        return s1max >= s2min;
                    } else {
                        return s2max >= s1min;
                    }
                }
                return false;
            };

            if(hasIntersection(playerBoundLeft, playerBoundRight, gapBoundLeft, gapBoundRight) &&
               !isSubset(gapBoundBottom, gapBoundTop, playerBoundBottom, playerBoundTop)) { //crashed
                playerWasHit = true;
                paused = true;
            } else {
                playerWasHit = false;
            }
            break;
        }
    }
    //crashing with floor and ceil
    playerHeight = std::clamp(playerHeight, -frameHeight / 2.0f + 0.5f, frameHeight / 2.0f - 0.5f);


    lastTick = std::chrono::steady_clock::now(); //update last tick
}

#include <glm/gtc/matrix_transform.hpp>

void FlappyBlock::render() {
    //render basic screen
    renderTarget->bind();

    glClearColor(0.5f, 1.0f, 0.5f, 1.0f); //(very) light green
    glClear(GL_COLOR_BUFFER_BIT);

    //walls
    for(auto &wall : walls) {
        shaderProgram.setUniform("color", glm::vec4(0.1f, 0.2f, 0.3f, 1.0f));
        shaderProgram.setUniform("modelView", glm::translate(glm::mat4(1.0f), glm::vec3(wall.distance - distance, 0.0f, 0.0f)));
        shaderProgram.use();
        wallsVA.drawArrays(GL_TRIANGLES, wall.memOffset / singleWallVideoDataSize * 6 * 2, 6 * 2);
    }

    //player
    glm::vec4 playerColor;
    if(playerWasHit) {
        playerColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else {
        //player's color will change while marching
        float green = distance / wallSpacing / 64.0f, blue = 0.0f;
        while(green > 1.0f) {
            blue += 1.0f / 64.0f;
            green -= 1.0f;
        }
        blue = std::clamp(blue, 0.0f, 1.0f);
        playerColor = glm::vec4(0.0f, green, blue, 1.0f);
    }
    shaderProgram.setUniform("color",playerColor);
    shaderProgram.setUniform("modelView", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, playerHeight, 0.0f)));
    shaderProgram.use();
    playerVA.drawArrays(GL_TRIANGLES, 0, 6);

    //composite
    renderTarget->unbind();
    renderTarget->bindTexture();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glm::vec4 blendColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if(paused) { //get darker when paused
        blendColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    }
    compositeProgram.setUniform("color", blendColor);
    compositeProgram.use();
    playerVA.drawArrays(GL_TRIANGLES, 0, 6);
}

void FlappyBlock::playerJump() {
    playerJumpFlag = true;
}

void FlappyBlock::onEsc() {
    escFlag = true;
}

void FlappyBlock::genWall() {
    static std::default_random_engine e(std::time(nullptr));
    static std::uniform_real_distribution<float> u(0.0f, 1.0f);

    double distance;
    if(walls.empty()) {
        distance = 10.0f;
    }
    else {
        distance = walls.back().distance + wallSpacing;
    }

    walls.push_back(Wall{distance, u(e)});

    //gen mesh
    GLuint offset = wallsVB.alloc();
    auto &wall = walls.back();
    wall.memOffset = offset;
    wallsVB.getInnerBuffer().map(GL_WRITE_ONLY, [&](void *data) {
       std::vector<float> vertices;
       auto top = frameHeight / 2.0f, bottom = -top;
       auto halfWallWidth = wallWidth / 2.0f;

       auto height = wall.height - 0.5f;
       auto gapTop = (frameHeight - 2 * wallPadding - wallGap / 2.0f) * height + wallGap / 2.0f;
       auto gapBottom = gapTop - wallGap;

       auto topWall = makeRect(-halfWallWidth, frameHeight / 2.0f, halfWallWidth, gapTop);
       auto bottomWall = makeRect(-halfWallWidth, gapBottom, halfWallWidth, -frameHeight / 2.0f);

       vertices.insert(vertices.end(), topWall.begin(), topWall.end());
       vertices.insert(vertices.end(), bottomWall.begin(), bottomWall.end());
       memcpy((uint8_t *)data + offset, vertices.data(), vertices.size() * sizeof(float));
    });
}

void FlappyBlock::resizeFrame(int width, int height) {
    //matrix
    float w = width, h = height;
    float ratio = w / h;

    /*float*/ halfWidth = ratio * frameHeight / 2.0f; //defined in class as shared data
    float halfHeight = frameHeight / 2.0f;
    shaderProgram.setUniform("projection", glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight));

    //viewport
    glViewport(0, 0, width, height);

    //framebuffer
    this->renderTarget.reset(new RenderTarget(width, height));
}

bool FlappyBlock::isWallDistanceVisible(double dist) const noexcept {
    return std::abs(distance - dist) /*distance to center*/ < wallWidth / 2.0f + halfWidth;
}

void FlappyBlock::reset() {
    walls.clear();
    distance = 0.0;
    playerHeight = 0.0f;
    playerSpeedY = 1.0f;
    paused = true;
    playerWasHit = false;
}

bool FlappyBlock::isPaused() {
    return paused;
}

