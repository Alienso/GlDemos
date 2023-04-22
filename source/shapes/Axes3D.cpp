//
// Created by Alienso on 26.9.2022..
//

#include "Axes3D.h"

Axes3D::Axes3D() {
    float vertices[] = { -1000.0f, 0.0f, 0.0f,
                         1000.0f, 0.0f, 0.0f,
                         0.0f, -1000.0f, 0.0f,
                         0.0f, 1000.0f, 0.0f,
                         0.0f, 0.0f, -1000.0f,
                         0.0f, 0.0f, 1000.0f };
    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(3);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
}

void Axes3D::render(glm::mat4& projection,glm::mat4& view, glm::mat4& model) {
    float lineWidth;
    glGetFloatv(GL_LINE_WIDTH,&lineWidth);
    glLineWidth(10);
    vb->bind();
    va->bind();
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model",model);
    shader->setVec3("uColor",1.0f,1.0f,1.0f);
    glDrawArrays(GL_LINES,0,6);
    glLineWidth;
    glLineWidth(lineWidth);
}

Axes3D::~Axes3D() {
    free(vb);
    free(va);
    free(layout);
    free(shader);
}
