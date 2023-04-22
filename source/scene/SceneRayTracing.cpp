//
// Created by Alienso on 23.9.2022..
//

#include <imgui/imgui.h>
#include "SceneRayTracing.h"
#include "../Configuration.h"

SceneRayTracing::SceneRayTracing() {
    shader = new Shader("rayTracing.vs", "rayTracing.fs");
    float vertices[] = {
            -1.0f, -1.0f,   // bottom right
            1.0f, -1.0f,    // bottom left
            1.0f, 1.0f,     // top
            -1.0f, 1.0f     //bottom
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 6);
}

SceneRayTracing::~SceneRayTracing() {
    free(shader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
}

void SceneRayTracing::onRender() {
    shader->setFloat("uTime",glfwGetTime());
    shader->setVec4("uMouse",0,0,0,1);
    shader->setVec2("uResolution",Configuration::wWidth,Configuration::wHeight);
    renderer.draw(*va,*ib,*shader);
}

void SceneRayTracing::onUpdate(float deltaTime) {

}

void SceneRayTracing::onImGuiRender() {
    ImGui::Begin("Hello, world!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
