//
// Created by Alienso on 14.4.2023..
//

#include "UniformBuffer.h"

#include "glad.h"

UniformBuffer::UniformBuffer() {
}

UniformBuffer::UniformBuffer(const void *data, unsigned int size){
    glGenBuffers(1,&rendererID);
    glBindBuffer(GL_UNIFORM_BUFFER,rendererID);
    glBufferData(GL_UNIFORM_BUFFER,size,data,GL_DYNAMIC_DRAW);
    //glBindBufferRange(GL_UNIFORM_BUFFER, /*buffer index*/, rendererID, 0, /*size*/); // this binds UBO to Buffer Index
}

UniformBuffer::~UniformBuffer() {
    glDeleteBuffers(1,&rendererID);
}

void UniformBuffer::bind() const {
    glBindBuffer(GL_UNIFORM_BUFFER,rendererID);
}

void UniformBuffer::unbind() const {
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}