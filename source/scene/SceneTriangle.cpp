//
// Created by Alienso on 23.9.2022..
//

#include <imgui/imgui.h>
#include "SceneTriangle.h"

SceneTriangle::SceneTriangle() {
    shader = new Shader("triangle.vs", "triangle.fs");
    float vertices[] = {
            -0.5f,-0.5f,    //bottom left
            0.5,-0.5f,      //bottom right
            0.0f,0.5f       //top
    };

    unsigned int indices[] = {
            0, 1, 2
    };

    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 3);
}

SceneTriangle::~SceneTriangle() {
    free(shader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
}

void SceneTriangle::onRender() {
    double timeValue = glfwGetTime();
    float r = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
    float g = static_cast<float>(sin(timeValue + 2.1f) / 2.0 + 0.5);
    float b = static_cast<float>(sin(timeValue + 4.2f) / 2.0 + 0.5);
    shader->setVec3("aColor", r, g, b);
    renderer.draw(*va,*ib,*shader);
}

void SceneTriangle::onUpdate(float deltaTime) {

}

void SceneTriangle::onImGuiRender() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
