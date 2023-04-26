//
// Created by Alienson on 26.4.2023..
//

#include "ShaderStorageBuffer.h"

#include "glad.h"

ShaderStorageBuffer::ShaderStorageBuffer() {
}

ShaderStorageBuffer::ShaderStorageBuffer(const void *data, unsigned int size){
    glGenBuffers(1,&rendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,rendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER,size,data,GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, rendererID); //this base must match layout in shader
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
    glDeleteBuffers(1,&rendererID);
}

void ShaderStorageBuffer::bind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,rendererID);
}

void ShaderStorageBuffer::unbind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
}