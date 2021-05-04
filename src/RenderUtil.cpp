#include "RenderUtil.h"

std::vector<float> makeRect(float left, float top, float right, float bottom) {
    return std::vector<float>{
        left, bottom,
        right, bottom,
        right, top, //right bottom
        left, bottom,
        right, top,
        left, top //left top
    };
}
