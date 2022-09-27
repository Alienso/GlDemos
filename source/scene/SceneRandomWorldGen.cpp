//
// Created by Alienson on 26.9.2022..
//

#include <imgui/imgui.h>
#include "SceneRandomWorldGen.h"
#include "../Configuration.h"
#include "../lib/glm/glm.hpp"
#include "../lib/glm//gtc/matrix_transform.hpp"
#include "../gen/MapGen.h"


SceneRandomWorldGen::SceneRandomWorldGen(GLFWwindow *Window) : window(Window) {
    glEnable(GL_DEPTH_TEST);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader = new Shader("perlin.vs", "perlin.fs");
    int width = 20,height = 20;

    float* vertices = (float*)calloc(width*height,sizeof(float)*6);
    unsigned int* indices = (unsigned int*)calloc((width-1)*(height-1),2*3*sizeof(unsigned int)); //2 triangles per square *3 vertexes per triangle

    MapGen mapGen(width,height,0.4,4,0.5f,2,1);
    mapGen.generateMap(&vertices,&indices);

    vb = new VertexBuffer(vertices, width*height*sizeof(float)*6);
    layout = new VertexBufferLayout();
    layout->push<float>(3);
    layout->push<float>(3);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices,(width-1)*(height-1)*2*3);

    free(vertices);
    free(indices);
}

SceneRandomWorldGen::~SceneRandomWorldGen() {
    free(shader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
    glDisable(GL_DEPTH_TEST);
}

void SceneRandomWorldGen::onRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(Configuration::fov), (float)Configuration::wWidth/(float)Configuration::wHeight,
                                            0.01f, 1000.0f);
    glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
    glm::mat4 model = glm::mat4(1.0f);

    axes3D.render(projection,view,model);
    shader->use();

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model",model);
    shader->setVec3("uColor",0.0f,1.0,0.0f);

    renderer.draw(*va,*ib,*shader);

    /*for (unsigned int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }*/
}

void SceneRandomWorldGen::onUpdate(float deltaTime) {
    processKeyboardInput(deltaTime);
    processMouseInput();
}

void SceneRandomWorldGen::onImGuiRender() {
    ImGui::Begin("Hello, world!");

    ImGui::Text("camera pos= %f,%f,%f", camera.pos[0],camera.pos[1],camera.pos[2]);
    ImGui::Text("pitch= %f, yaw = %f", camera.pitch, camera.yaw);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void SceneRandomWorldGen::processMouseInput() {
    double xposIn,yposIn;
    glfwGetCursorPos(window,&xposIn,&yposIn);

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (camera.firstMouse)
    {
        camera.lastPos[0] = xpos;
        camera.lastPos[1] = ypos;
        camera.firstMouse = false;
    }

    float xoffset = xpos - camera.lastPos[0];
    float yoffset = camera.lastPos[1] - ypos;
    camera.lastPos[0] = xpos;
    camera.lastPos[1] = ypos;

    float sensitivity = 0.1f;
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
}

void SceneRandomWorldGen::processKeyboardInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
        int mode = glfwGetInputMode(window,GLFW_CURSOR);
        glfwSetInputMode(window, GLFW_CURSOR, mode == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        camera.firstMouse = true;
    }

    float cameraSpeed = static_cast<float>(20 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::vec3(0.0f,1.0f,0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.pos += cameraSpeed * glm::vec3(0.0f,-1.0f,0.0f);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    Configuration::fov -= (float)yoffset;
    if (Configuration::fov < 1.0f)
        Configuration::fov = 1.0f;
    if (Configuration::fov > Configuration::fovMax)
        Configuration::fov = Configuration::fovMax;
}
