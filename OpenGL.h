#ifndef FLAPPY_BLOCK_OPENGL_H
#define FLAPPY_BLOCK_OPENGL_H
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <functional>
#include <filesystem>

class VertexArray {
public:
    VertexArray();
    VertexArray(VertexArray &&);
    ~VertexArray();

    void setAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, const void *offset);
    void enableAttribute(GLuint index);

    void bind();
    static void unbind();
private:
    GLuint object;
};

class VertexBuffer {
public:
    VertexBuffer(GLenum usage, GLsizei size);
    VertexBuffer(VertexBuffer &&);
    ~VertexBuffer();

    void map(GLenum access, std::function<void(void *data)> callback);

    void bind(GLenum target);
    static void unbind(GLenum target);

private:
    GLuint object;
};

class ShaderProgram {
private:
    ShaderProgram(GLuint object);
public:
    ShaderProgram loadFromFile(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath);
    ShaderProgram(ShaderProgram &&);
    ~ShaderProgram();

    void setUniform(const std::string &var, const glm::vec4 &val);
    void serUniform(const std::string &var, const glm::vec4 &val);

    void use();
    static void disuse();
private:
    GLuint object;
};

#endif //FLAPPY_BLOCK_OPENGL_H
