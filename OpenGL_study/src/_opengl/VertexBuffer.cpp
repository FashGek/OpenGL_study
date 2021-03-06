#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, const unsigned int& size)
    : renderer_id_(0)
{
    GLCall(glGenBuffers(1, &renderer_id_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    : renderer_id_(other.renderer_id_)
{
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &renderer_id_));
    unbind();
    renderer_id_= 0;
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
