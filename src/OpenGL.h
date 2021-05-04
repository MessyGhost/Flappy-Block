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

    void setAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, const void *offset) const;
    void enableAttribute(GLuint index) const;

    void bind() const;
    static void unbind();

    void drawArrays(GLenum mode, GLuint first, GLsizei count) const;

private:
    GLuint object;
};

class VertexBuffer {
public:
    VertexBuffer(GLenum usage, GLsizei size);
    VertexBuffer(VertexBuffer &&);
    ~VertexBuffer();

    GLuint size() const noexcept;

    void map(GLenum access, std::function<void(void *data)> callback) const;

    void bind(GLenum target) const;
    static void unbind(GLenum target);

private:
    GLuint object;
    GLuint capacity;
};

class ShaderProgram {
private:
    ShaderProgram(GLuint object);
public:
    static ShaderProgram loadFromFile(const std::filesystem::path &vertexShaderPath, const std::filesystem::path &fragmentShaderPath);
    ShaderProgram(ShaderProgram &&);
    ~ShaderProgram();

    void setUniform(const std::string &var, const glm::vec4 &val) const;
    void setUniform(const std::string &var, const glm::mat4 &val) const;

    void use() const;
    static void disuse();
private:
    GLuint object;
};

#endif //FLAPPY_BLOCK_OPENGL_H
