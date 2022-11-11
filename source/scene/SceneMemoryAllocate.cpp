//
// Created by arsav on 11.11.2022..
//

#include "SceneMemoryAllocate.h"

SceneMemoryAllocate::SceneMemoryAllocate(int size_) {
    std::cout<<size_<<'\n';
    int size = 1024*1024*size_;
    shader = new Shader("triangle.vs","triangle.fs");
    float* data = (float*)malloc(size);
    unsigned int indices[] = {
            0, 1, 2
    };
    vb = new VertexBuffer(data, size);
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices,3);
}

SceneMemoryAllocate::~SceneMemoryAllocate() {
    free(shader);
    free(va);
    free(vb);
    free(ib);
    free(layout);
}

void SceneMemoryAllocate::onRender() {
    shader->use();
    shader->setVec3("aColor",1.0,1.0,1.0);

    renderer.draw(*va,*ib,*shader);
}

void SceneMemoryAllocate::onUpdate(float deltaTime) {

}

void SceneMemoryAllocate::onImGuiRender() {

}
