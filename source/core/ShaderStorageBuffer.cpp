//
// Created by Alienso on 26.4.2023..
//

#include "ShaderStorageBuffer.h"

#include "glad.h"

ShaderStorageBuffer::ShaderStorageBuffer() {
}

ShaderStorageBuffer::ShaderStorageBuffer(const void *data, unsigned int size){
    glGenBuffers(1,&rendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,rendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER,size,data,GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, rendererID); //this base must match layout in shader
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

ShaderStorageBuffer::ShaderStorageBuffer(std::vector<Mesh *> &meshes) {
    glGenBuffers(1,&rendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,rendererID);

    unsigned long long totalSize = 0;
    for (auto &mesh : meshes){
        totalSize += mesh->triangles.size() * sizeof(Triangle);
    }
    glBufferData(GL_SHADER_STORAGE_BUFFER, totalSize, nullptr, GL_DYNAMIC_DRAW);

    unsigned long long offset = 0;
    for (auto &mesh : meshes){
        unsigned long long size = mesh->triangles.size() * sizeof(Triangle);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER,offset,size,(void*)&((mesh->triangles)[0]));
        offset += size;
    }

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

void ShaderStorageBuffer::updateData(std::vector<Mesh*> &meshes){
    bind();

    unsigned long long offset = 0;
    for (auto &mesh : meshes){
        unsigned long long size = mesh->triangles.size() * sizeof(Triangle);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER,offset,size,(void*)&((mesh->triangles)[0]));
        offset += size;
    }

    unbind();
}
