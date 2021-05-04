cmake_minimum_required(VERSION 3.19)
project(flappy_block)

set(CMAKE_CXX_STANDARD 17)

add_executable(flappy_block main.cpp OpenGL.cpp OpenGL.h FlappyBlock.cpp FlappyBlock.h RenderUtil.h RenderUtil.cpp BlockyVideoMemoryPool.cpp BlockyVideoMemoryPool.h)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

target_include_directories(flappy_block PRIVATE ${GLEW_INCLUDE_DIRS} PRIVATE ${GLFW_INCLUDE_DIRS} PRIVATE ${GLM_INCLUDE_DIRS})
if(WIN32)
    set(OPENGL_LIB opengl32)
else()
    set(OPENGL_LIB GL)
endif()
target_link_libraries(flappy_block PRIVATE GLEW::GLEW PRIVATE glfw PRIVATE glm PRIVATE ${OPENGL_LIB})
