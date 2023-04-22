//
// Created by Alienso on 12.4.2023..
//

#include <imgui/imgui.h>
#include "SceneAdvancedRayTracing.h"


#include "../Configuration.h"
#include "../util/RayTracingMaterials.h"
#include "../core/Texture.h"


SceneAdvancedRayTracing::SceneAdvancedRayTracing(GLFWwindow* _window) : window(_window) {
    glEnable( GL_TEXTURE_2D );

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

    glDisable( GL_TEXTURE_2D );
}

void SceneAdvancedRayTracing::setupSpheres() {
    spheres[0] = Sphere(glm::vec3(sunX, sunY, sunZ), 35.0, RayTracingMaterials::light);
    spheres[1] = Sphere(glm::vec3(0, 0, 5), 2.0, RayTracingMaterials::green);
    spheres[2] = Sphere(glm::vec3(0, 8, 15), 10.0, RayTracingMaterials::red);
    spheres[3] = Sphere(glm::vec3(0.0, -1002.0, 0.0), 1000, RayTracingMaterials::blue);
}

void SceneAdvancedRayTracing::SetUniformSpheres(const std::string& name, Sphere* array, int size){
    for (int i=0; i<size; i++){
        glUniform3f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].position"),array[i].position.x, array[i].position.y, array[i].position.z);
        glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].radius"), array[i].radius);

        glUniform4f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.color"), array[i].material.color.x,array[i].material.color.y,array[i].material.color.z,array[i].material.color.w);
        glUniform3f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.emissionColor"), array[i].material.emissionColor.x,array[i].material.emissionColor.y,array[i].material.emissionColor.z);
        glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.emissionStrength"), array[i].material.emissionStrength);
        glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.smoothness"), array[i].material.smoothness);
    }
}

void SceneAdvancedRayTracing::onRender() {

    shader->use();
    fb->bind();

    shader->setFloat("uTime", glfwGetTime());
    shader->setVec4("uMouse",0,0,0,1);
    shader->setVec2("uResolution",Configuration::wWidth,Configuration::wHeight);

    shader->setInt("uSphereCount", 4);
    SetUniformSpheres("uSpheres", spheres, 4);
    shader->setUInt("uFramesRendered", framesRendered);
    screenShader->setInt("uResetBuffer",resetFrameBuffer);

    renderer.draw(*va,*ib,*shader);

    fb->unbind();
    fb->bindTexture();
    screenShader->use();

    renderer.draw(*va, *ib, *screenShader);

    framesRendered++;
}

void SceneAdvancedRayTracing::onUpdate(float deltaTime) {
    processMouseInput();

    spheres[0].position = glm::vec3(sunX, sunY, sunZ);
    spheres[0].material.emissionStrength = sunStrength;

    resetFrameBuffer = 0;
}

void SceneAdvancedRayTracing::onImGuiRender() {
    ImGui::Begin("Ray Tracing Demo");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    createWidget(ImGui::SliderFloat("X",&sunX,-200,200));
    createWidget(ImGui::SliderFloat("Y",&sunY,-200,200));
    createWidget(ImGui::SliderFloat("Z",&sunZ,-200,200));
    createWidget(ImGui::SliderFloat("Strength", &sunStrength, -1, 10));

    ImGui::End();

    if(selectedSphere != nullptr){
        ImGui::Begin("Sphere");
        createWidget(ImGui::SliderFloat("X",&selectedSphere->position.x,-200,200));
        createWidget(ImGui::SliderFloat("Y",&selectedSphere->position.y,-200,200));
        createWidget(ImGui::SliderFloat("Z",&selectedSphere->position.z,-200,200));
        ImGui::End();
    }
}

void SceneAdvancedRayTracing::createWidget(bool valueChanged){
    if (valueChanged){
        resetFrameBuffer = 1;
        framesRendered = 0;
    }
}

void SceneAdvancedRayTracing::trace(double xPos, double yPos){
    float minDist = 99999;

    glm::vec2 uv = glm::vec2(xPos,yPos)/glm::vec2(Configuration::wWidth,Configuration::wHeight);

    uv = uv * glm::vec2(2.0) - glm::vec2(1.0);//transform from [0,1] to [-1,1]
    uv.x *= (float)(Configuration::wWidth)/(float)Configuration::wHeight; //aspect fix

    glm::vec3 cameraPos = glm::vec3(0,6.0,-25.0);
    glm::vec3 cameraFocusPoint = glm::vec3(0,0,0);
    glm::vec3 cameraDir = normalize(cameraFocusPoint - cameraPos);
    glm::vec3 rayDir = normalize(cameraDir + glm::vec3(uv.x,uv.y,0));

    for (int i=0; i< 4; i++){
        float dist = raySphere(cameraPos, rayDir, spheres[i].position, spheres[i].radius);

        if (dist >= 0 && dist < minDist){
            selectedSphere = &(spheres[i]);
            minDist = dist;
        }
    }
}

float SceneAdvancedRayTracing::raySphere(glm::vec3& rayOrigin, glm::vec3& rayDir, glm::vec3& sphereCenter, float sphereRadius){
    float ret = -1;
    glm::vec3 offsetRayOrigin = rayOrigin - sphereCenter;

    float a = dot(rayDir, rayDir);
    float b = 2 * dot(offsetRayOrigin, rayDir);
    float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    //if discriminant is < 0, then we didnt hit the circle
    if (discriminant >= 0){
        float dist = (-b - sqrt(discriminant)) / (2 * a);
        if (dist > 0){
            ret = dist;
        }
    }
    return ret;
}

void SceneAdvancedRayTracing::processMouseInput(){
    if (ImGui::IsAnyItemActive())
        return;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        trace(xpos,Configuration::wHeight - ypos);
    }
}