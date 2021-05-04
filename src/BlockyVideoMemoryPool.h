#ifndef FLAPPY_BLOCK_BLOCKYVIDEOMEMORYPOOL_H
#define FLAPPY_BLOCK_BLOCKYVIDEOMEMORYPOOL_H
#include "OpenGL.h"
#include <deque>
#include <memory>
#include <functional>

class BlockyVideoMemoryPool {
public:
    BlockyVideoMemoryPool(GLuint blockSize);
    BlockyVideoMemoryPool(BlockyVideoMemoryPool &&);
    ~BlockyVideoMemoryPool();
    /*!
     * @result Offset of the inner buffer;
     */
    GLuint alloc();
    /*!
     * @param[in] offset Offset of the inner buffer (which alloc() returns).
     */
    void free(GLuint offset);

    void setOnRealloc(std::function<void(const VertexBuffer &)> callback);

    const VertexBuffer &getInnerBuffer() const noexcept;
private:
    void realloc(GLuint capacity);
private:
    std::unique_ptr<VertexBuffer> pbuffer;
    GLuint blockSize, bufferSize, capacity;
    std::deque<bool> usage;
    std::function<void(const VertexBuffer &)> onRealloc;
};


#endif //FLAPPY_BLOCK_BLOCKYVIDEOMEMORYPOOL_H
