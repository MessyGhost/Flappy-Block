cmake_minimum_required(VERSION 3.19)
project(flappy_block)

set(CMAKE_CXX_STANDARD 17)

add_executable(flappy_block main.cpp OpenGL.cpp OpenGL.h FlappyBlock.cpp FlappyBlock.h RenderUtil.h RenderUtil.cpp)

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(flappy_block PRIVATE GLEW::GLEW PRIVATE glfw PRIVATE glm)
