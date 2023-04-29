//
// Created by Alienso on 12.4.2023..
//

#include <imgui/imgui.h>
#include "SceneAdvancedRayTracing.h"


#include "../Configuration.h"
#include "rayTracing/RayTracingMaterials.h"
#include "../core/Texture.h"
#include "../util/Profiler.h"


SceneAdvancedRayTracing::SceneAdvancedRayTracing(GLFWwindow* _window) : window(_window) {

    Instrumentor::get().beginSession("Main");
    PROFILE_SCOPE("Constructor");

    glEnable( GL_TEXTURE_2D );

    shader = new Shader("rayTracingAdvanced.vs", "rayTracingAdvanced.fs");
    screenShader = new Shader("frameBuffer.vs", "frameBuffer.fs");
    camera.pos = glm::vec3(0,6.0,-25.0);
    camera.front = glm::vec3(0.0,-0.2334,0.97);
    camera.yaw = -280;

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

    setupSpheresBasic();
    //setupSpheresReflectingWalls();
    //setupSpheresReflectingOrbs();
    //setupModel();

    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 6);
    fb = new FrameBuffer();
    //ssb = new ShaderStorageBuffer((void*)&((mesh->triangles)[0]), (mesh->triangles).size() * sizeof(Triangle));

}

SceneAdvancedRayTracing::~SceneAdvancedRayTracing() {
    free(shader);
    free(screenShader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
    free(fb);
    free(ssb);

    Instrumentor::get().endSession();

    glDisable( GL_TEXTURE_2D );
}

void SceneAdvancedRayTracing::onRender() {

    PROFILE_SCOPE("onRender");

    shader->use();
    fb->bind();

    shader->setFloat("uTime", glfwGetTime());
    shader->setVec4("uMouse",0,0,0,1);
    shader->setVec2("uResolution",Configuration::wWidth,Configuration::wHeight);
    shader->setVec3("uCameraPos", camera.pos);
    shader->setVec3("uCameraFocusPoint", camera.front);

    shader->setInt("uSphereCount", spheres.size());
    SetUniformSpheres(spheres);
    shader->setUInt("uFramesRendered", framesRendered);
    shader->setInt("uResetBuffer", resetFrameBuffer);

    shader->setInt("uMeshCount",1);
    setUniformMeshInfo(meshArray);

    //ssb->bind();

    renderer.draw(*va,*ib,*shader);

    fb->unbind();
    fb->bindTexture();
    screenShader->use();

    renderer.draw(*va, *ib, *screenShader);

    framesRendered++;
}

void SceneAdvancedRayTracing::onUpdate(float deltaTime) {
    PROFILE_SCOPE("onUpdate");
    //processMouseClick(); //todo temp moved to onImGuiRender
    // processMouseInput();
    processKeyboardInput(deltaTime);
    resetFrameBuffer = 0;
}

void SceneAdvancedRayTracing::onImGuiRender() {
    PROFILE_SCOPE("onImGuiRender");
    ImGui::Begin("Ray Tracing Demo");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    if(selectedSphere != nullptr){
        createWidget(ImGui::SliderFloat("X",&selectedSphere->position.x,-50,50));
        createWidget(ImGui::SliderFloat("Y",&selectedSphere->position.y,-50,50));
        createWidget(ImGui::SliderFloat("Z",&selectedSphere->position.z,-50,50));

        createWidget(ImGui::InputFloat("Radius",&selectedSphere->radius,0.1f,1));
        createWidget(ImGui::ColorEdit3("Color", &selectedSphere->material.color.x));
        createWidget(ImGui::ColorEdit3("Emission Color", &selectedSphere->material.emissionColor.x));
        createWidget(ImGui::InputFloat("Emission Strength",&selectedSphere->material.emissionStrength, 0.1f, 1));
        createWidget(ImGui::SliderFloat("Smoothness",&selectedSphere->material.smoothness, 0, 1));
        createWidget(ImGui::SliderFloat("Specular probability",&selectedSphere->material.specularProbability, 0, 1));
        createWidget(ImGui::SliderInt("Invisible light source",&selectedSphere->material.isInvisibleLightSource,0,1));
        createWidget(ImGui::SliderFloat("Transparency",&selectedSphere->material.transparency, 0, 1));
        createWidget(ImGui::SliderFloat("Refractive Index",&selectedSphere->material.refractionIndex, 0, 20));
    }

    if (selectedTriangle != nullptr){
        createWidget(ImGui::SliderFloat("PosA X",&selectedTriangle->posA.x,-50,50));
        createWidget(ImGui::SliderFloat("PosA Y",&selectedTriangle->posA.y,-50,50));
        createWidget(ImGui::SliderFloat("PosA Z",&selectedTriangle->posA.z,-50,50));

        createWidget(ImGui::SliderFloat("PosB X",&selectedTriangle->posB.x,-50,50));
        createWidget(ImGui::SliderFloat("PosB Y",&selectedTriangle->posB.y,-50,50));
        createWidget(ImGui::SliderFloat("PosB Z",&selectedTriangle->posB.z,-50,50));

        createWidget(ImGui::SliderFloat("PosC X",&selectedTriangle->posC.x,-50,50));
        createWidget(ImGui::SliderFloat("PosC Y",&selectedTriangle->posC.y,-50,50));
        createWidget(ImGui::SliderFloat("PosC Z",&selectedTriangle->posC.z,-50,50));

        //TODO FIX NORMALS
    }

    if (!ImGui::IsAnyItemActive()){
        processMouseClick();
    }

    ImGui::End();
}

void SceneAdvancedRayTracing::createWidget(bool valueChanged){
   if (valueChanged){
       resetFrameBuffer = 1;
       framesRendered = 0;
   }
}


void SceneAdvancedRayTracing::processMouseClick(){
   if (ImGui::IsAnyItemActive())
       return;
   if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
       double xpos, ypos;
       glfwGetCursorPos(window, &xpos, &ypos);
       trace(xpos,Configuration::wHeight - ypos);
   }
}

void SceneAdvancedRayTracing::processKeyboardInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
    }

    glm::vec3 oldPos = camera.pos;
    float cameraSpeed = static_cast<float>(20 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::vec3(0.0f,1.0f,0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::vec3(0.0f,-1.0f,0.0f);

    if (oldPos != camera.pos){
        resetFrameBuffer = 1;
        framesRendered = 0;
    }
}

void SceneAdvancedRayTracing::processMouseInput() {

    double xposIn,yposIn;
    glfwGetCursorPos(window,&xposIn,&yposIn);

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse){
        camera.lastPos[0] = xpos;
        camera.lastPos[1] = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastPos[0];
    float yoffset = camera.lastPos[1] - ypos;
    camera.lastPos[0] = xpos;
    camera.lastPos[1] = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS){
        return;
    }

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);

    resetFrameBuffer = 1;
    framesRendered = 0;
}