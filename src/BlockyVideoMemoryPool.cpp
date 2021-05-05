#include "BlockyVideoMemoryPool.h"
#include <cstring>

BlockyVideoMemoryPool::BlockyVideoMemoryPool(GLuint blockSize)
:blockSize(blockSize)
{
    realloc(8);
}

BlockyVideoMemoryPool::BlockyVideoMemoryPool(BlockyVideoMemoryPool &&rhs) {
    this->pbuffer.swap(rhs.pbuffer);
    this->usage.swap(rhs.usage);
    this->blockSize = rhs.blockSize;
    this->capacity = rhs.capacity;
    this->bufferSize = rhs.bufferSize;
}

BlockyVideoMemoryPool::~BlockyVideoMemoryPool() {

}

GLuint BlockyVideoMemoryPool::alloc() {
    int a = 2;
    size_t index = SIZE_MAX;
    while(a-- > 0) {
        size_t i = 0;
        for(auto used : usage) { //search unused block
            if(!used) {
                index = i;
                break;
            }
            ++i;
        }

        if(index == SIZE_MAX){  //failed
            realloc(this->capacity * 2);
        }
        else { //succeed
            usage[index] = true;
            break;
        }
    }
    return index * blockSize;
}

void BlockyVideoMemoryPool::free(GLuint offset) {
    if(offset % blockSize != 0) {
        throw std::runtime_error("Isn't packed");
    }
    usage[offset / blockSize] = false;
}

void BlockyVideoMemoryPool::setOnRealloc(std::function<void(const VertexBuffer &)> callback) {
    onRealloc = callback;
}

void BlockyVideoMemoryPool::realloc(GLuint capacity) {
    std::unique_ptr<VertexBuffer> newBuffer(new VertexBuffer(GL_DYNAMIC_DRAW, capacity * blockSize));
    if(pbuffer) {
        auto *buffer = std::malloc(bufferSize);
        pbuffer->map(GL_READ_ONLY, [&](void *data) {
           std::memcpy(buffer, data, bufferSize);
        });

        newBuffer->map(GL_WRITE_ONLY, [&](void *data) {
           std::memcpy(data, buffer, bufferSize);
        });
        std::free(buffer);
    }
    pbuffer.swap(newBuffer);

    this->capacity = capacity;
    this->bufferSize = pbuffer->size();
    this->usage.resize(capacity, false);

    if(onRealloc) {
        onRealloc(getInnerBuffer());
    }
}

const VertexBuffer &BlockyVideoMemoryPool::getInnerBuffer() const noexcept {
    return *pbuffer;
}
