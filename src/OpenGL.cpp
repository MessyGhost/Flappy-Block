#include "OpenGL.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

VertexArray::VertexArray() {
    glGenVertexArrays(1, &object);
}

VertexArray::VertexArray(VertexArray &&rhs) {
    this->object = rhs.object;
    rhs.object = 0;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &object);

}

void VertexArray::setAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, const void *offset) const {
    bind();
    glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
    unbind();
}

void VertexArray::enableAttribute(GLuint index) const {
    bind();
    glEnableVertexAttribArray(index);
    unbind();
}

void VertexArray::bind() const {
    glBindVertexArray(object);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

void VertexArray::drawArrays(GLenum mode, GLuint first, GLsizei count) const {
    bind();
    glDrawArrays(mode, first, count);
    unbind();
}

VertexBuffer::VertexBuffer(GLenum usage, GLsizei size) {
    glGenBuffers(1, &object);
    bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
    unbind(GL_ARRAY_BUFFER);
}

VertexBuffer::VertexBuffer(VertexBuffer &&rhs) {
    this->object = rhs.object;
    rhs.object = 0;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &object);
}

void VertexBuffer::map(GLenum access, std::function<void(void *)> callback) const {
    bind(GL_ARRAY_BUFFER);
    void *data;
    data = glMapBuffer(GL_ARRAY_BUFFER, access);
    callback(data);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    unbind(GL_ARRAY_BUFFER);
}

void VertexBuffer::bind(GLenum target) const {
    glBindBuffer(target, object);
}

void VertexBuffer::unbind(GLenum target) {
    glBindBuffer(target, 0);
}

ShaderProgram::ShaderProgram(GLuint object)
    :object(object)
{

}

ShaderProgram ShaderProgram::loadFromFile(const std::filesystem::path &vertexShaderPath,
                                          const std::filesystem::path &fragmentShaderPath) {
    std::ifstream vertexShaderFile(vertexShaderPath), fragmentShaderFile(fragmentShaderPath);
    GLuint program; //program object
    try {
        if(!vertexShaderFile || !fragmentShaderFile) {
            vertexShaderFile.close();
            fragmentShaderFile.close();
            throw std::runtime_error("Cannot read shader files.");
        }

        auto readFile = [&](std::ifstream &in) -> std::string {
            std::stringstream bufferStream;
            bufferStream << in.rdbuf();

            return bufferStream.str();
        };

        auto compileShader = [&](GLuint shader, const std::string &source) -> void {
            auto ptr = source.c_str();
            glShaderSource(shader, 1, &ptr, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(success != GL_TRUE) {
                throw std::runtime_error("Cannot pass shader compilation.");
            }
        };

        auto vertexShaderSource = readFile(vertexShaderFile);
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        try {
            compileShader(vertexShader, vertexShaderSource);

            auto fragmentShaderSource = readFile(fragmentShaderFile);
            auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            try {
                compileShader(fragmentShader, fragmentShaderSource);

                program = glCreateProgram();
                glAttachShader(program, vertexShader);
                glAttachShader(program, fragmentShader);

                glLinkProgram(program);
                GLint success;
                glGetProgramiv(program, GL_LINK_STATUS, &success);
                if(success != GL_TRUE) {
                    char log[512];
                    glGetProgramInfoLog(program, 512, nullptr, log);
                    glDeleteProgram(program);
                    throw std::runtime_error("Cannot link program.");
                }
            }
            catch(...) {
                glDeleteShader(fragmentShader);
                throw;
            }
            glDeleteShader(fragmentShader);
        }
        catch(...) {
            glDeleteShader(vertexShader);
            throw;
        }
        glDeleteShader(vertexShader);
    }
    catch (...) {
        vertexShaderFile.close();
        fragmentShaderFile.close();
        throw;
    }
    vertexShaderFile.close();
    fragmentShaderFile.close();
    return ShaderProgram(program);
}

ShaderProgram::ShaderProgram(ShaderProgram &&rhs) {
    this->object = rhs.object;
    rhs.object = 0;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(object);
}

#include <glm/gtc/type_ptr.hpp>

void ShaderProgram::setUniform(const std::string &var, const glm::vec4 &val) const {
    use();
    glUniform4fv(glGetUniformLocation(object, var.c_str()), 1, glm::value_ptr(val));
    disuse();
}

void ShaderProgram::setUniform(const std::string &var, const glm::mat4 &val) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(object, var.c_str()), 1, GL_FALSE, glm::value_ptr(val));
    disuse();
}

void ShaderProgram::use() const {
    glUseProgram(object);
}

void ShaderProgram::disuse() {
    glUseProgram(0);
}
