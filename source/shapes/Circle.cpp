//
// Created by Alienso on 26.9.2022..
//

#include "Circle.h"

Circle::Circle(float R, float r, float g, float b) {
    float vertices[] = {0.0f,0.0f,0.0f};
    unsigned int indices[] = {0};
    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(3);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 3);
}

void Circle::render(glm::mat4& projection,glm::mat4& view, glm::mat4& model) {
    renderer.draw(*va,*ib,*shader);
}

Circle::~Circle() {
    free(vb);
    free(ib);
    free(va);
    free(layout);
    free(shader);
}
