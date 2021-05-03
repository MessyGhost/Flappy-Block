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

void VertexArray::setAttribute(GLuint index, GLuint size, GLenum type, bool normalized, GLuint stride, const void *offset) {
    bind();
    glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
    unbind();
}

void VertexArray::enableAttribute(GLuint index) {
    bind();
    glEnableVertexAttribArray(index);
    unbind();
}

void VertexArray::bind() {
    glBindVertexArray(object);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

VertexBuffer::VertexBuffer(GLenum usage, GLsizei size) {
    glGenBuffers(1, &object);
}

VertexBuffer::VertexBuffer(VertexBuffer &&rhs) {
    this->object = rhs.object;
    rhs.object = 0;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &object);
}

void VertexBuffer::map(GLenum access, std::function<void(void *)> callback) {
    bind(GL_ARRAY_BUFFER);
    void *data;
    data = glMapBuffer(GL_ARRAY_BUFFER, access);
    callback(data);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    unbind(GL_ARRAY_BUFFER);
}

void VertexBuffer::bind(GLenum target) {
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
        }

        auto readFile = [&](std::ifstream &in) -> std::string {
            std::stringstream bufferStream;
            bufferStream << in.rdbuf();

            return bufferStream.str();
        };

        auto compileShader = [&](GLuint shader, const std::string &source) -> void {
            glShaderSource(shader, 1, (const GLchar *const *)source.c_str(), nullptr);
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
                    glDeleteProgram(program);
                    throw std::runtime_error("Cannot link program.");
                }
            }
            catch(...) {
                glDeleteShader(fragmentShader);
                throw;
            }
        }
        catch(...) {
            glDeleteShader(vertexShader);
            throw;
        }
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

ShaderProgram::ShaderProgram(ShaderProgram &&) {

}

ShaderProgram::~ShaderProgram() {

}

void ShaderProgram::setUniform(const std::string &var, const glm::vec4 &val) {

}

void ShaderProgram::serUniform(const std::string &var, const glm::vec4 &val) {

}

void ShaderProgram::use() {

}

void ShaderProgram::disuse() {

}
