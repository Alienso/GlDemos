//
// Created by Alienso on 12.4.2023..
//

#include <imgui/imgui.h>
#include "SceneAdvancedRayTracing.h"


#include "../Configuration.h"
#include "../util/RayTracingMaterials.h"
#include "../core/Texture.h"


SceneAdvancedRayTracing::SceneAdvancedRayTracing() {
    shader = new Shader("rayTracingAdvanced.vs", "rayTracingAdvanced.fs");
    screenShader = new Shader("frameBuffer.vs", "frameBuffer.fs");

    float vertices[] = {
            // positions   // texCoords
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f,  0.0f, 1.0f,
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    setupSpheres();

    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 6);
    fb = new FrameBuffer();

}

SceneAdvancedRayTracing::~SceneAdvancedRayTracing() {
    free(shader);
    free(screenShader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
    free(fb);
}

void SceneAdvancedRayTracing::setupSpheres() {
    Spheres[0] = Sphere(glm::vec3(0,8,15),10.0, RayTracingMaterials::red);
    Spheres[1] = Sphere(glm::vec3(0,0,5),2.0, RayTracingMaterials::green);
    Spheres[2] = Sphere(glm::vec3(sunX,sunY,sunZ),35.0, RayTracingMaterials::light);
    Spheres[3] = Sphere(glm::vec3(0.0,-1002.0,0.0),1000, RayTracingMaterials::blue);
}

void SceneAdvancedRayTracing::SetUniformSpheres(const std::string& name, Sphere* array, int size){
    for (int i=0; i<size; i++){

        glUniform3f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].position").c_str()), array[i].position.x, array[i].position.y, array[i].position.z);
        glUniform1f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].radius").c_str()), array[i].radius);

        glUniform4f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].material.color").c_str()), array[i].material.color.x,array[i].material.color.y,array[i].material.color.z,array[i].material.color.w);
        glUniform3f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].material.emissionColor").c_str()), array[i].material.emissionColor.x,array[i].material.emissionColor.y,array[i].material.emissionColor.z);
        glUniform1f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].material.emissionStrength").c_str()), array[i].material.emissionStrength);
        glUniform1f(glGetUniformLocation(shader->ID, (name + "[" + std::to_string(i) + "].material.smoothness").c_str()), array[i].material.smoothness);
    }
}

void SceneAdvancedRayTracing::onRender() {

    shader->use();
    fb->bind();

    shader->setFloat("uTime", glfwGetTime());
    shader->setVec4("uMouse",0,0,0,1);
    shader->setVec2("uResolution",Configuration::wWidth,Configuration::wHeight);

    shader->setInt("uSphereCount", 4);
    SetUniformSpheres("uSpheres", Spheres, 4);
    shader->setUInt("uFramesRendered", framesRendered);
    screenShader->setInt("uResetBuffer",resetFrameBuffer);

    renderer.draw(*va,*ib,*shader);

    fb->unbind();
    fb->bindTexture();
    screenShader->use();

    renderer.draw(*va, *ib, *screenShader);
}

void SceneAdvancedRayTracing::onUpdate(float deltaTime) {
    Spheres[2].position = glm::vec3(sunX,sunY,sunZ);
    Spheres[2].material.emissionStrength = sunStrength;

    resetFrameBuffer = 0;
    framesRendered++;
}

void SceneAdvancedRayTracing::onImGuiRender() {
    ImGui::Begin("Ray Tracing Demo");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    createWidget(ImGui::SliderFloat("X",&sunX,-200,200));
    createWidget(ImGui::SliderFloat("Y",&sunY,-200,200));
    createWidget(ImGui::SliderFloat("Z",&sunZ,-200,200));
    createWidget(ImGui::SliderFloat("Strength", &sunStrength, -1, 10));

    ImGui::End();
}

void SceneAdvancedRayTracing::createWidget(bool valueChanged){
    if (valueChanged){
        resetFrameBuffer = 1;
        framesRendered = 0;
    }
}
