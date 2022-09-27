//
// Created by Alienson on 23.9.2022..
//

#include <imgui/imgui.h>
#include "Scene3D.h"
#include "../lib/glm/glm.hpp"
#include "../lib/glm//gtc/matrix_transform.hpp"
#include "../lib/glm//gtc/type_ptr.hpp"
#include "../Configuration.h"

Scene3D::Scene3D(GLFWwindow* Window) : window(Window) {

    glEnable(GL_DEPTH_TEST);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader = new Shader("3D.vs", "3D.fs");
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    cubePositions[0] = glm::vec3( 0.0f,  0.0f,  0.0f);
    cubePositions[1] = glm::vec3( 2.0f,  5.0f, -15.0f);
    cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
    cubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
    cubePositions[4] = glm::vec3( 2.4f, -0.4f, -3.5f);
    cubePositions[5] = glm::vec3(-1.7f,  3.0f, -7.5f);
    cubePositions[6] = glm::vec3( 1.3f, -2.0f, -2.5f);
    cubePositions[7] = glm::vec3( 1.5f,  2.0f, -2.5f);
    cubePositions[8] = glm::vec3( 1.5f,  0.2f, -1.5f);
    cubePositions[9] = glm::vec3(-1.3f,  1.0f, -1.5f);


    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(3);
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    texture1 = new Texture ("Background.jpg",GL_RGB);
    texture2 = new Texture ("Background.jpg",GL_RGB);

    shader->use();
    shader->setInt("texture1",0);
    shader->setInt("texture2",1);
}

Scene3D::~Scene3D() {
    free(shader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
    free(texture1);
    free(texture2);
    glDisable(GL_DEPTH_TEST);
}

void Scene3D::onRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(Configuration::fov), (float)Configuration::wWidth/(float)Configuration::wHeight,
                                            0.01f, 1000.0f);
    glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
    glm::mat4 model = glm::mat4(1.0f);

    axes3D.render(projection,view,model);
    texture1->bind(0);
    texture2->bind(1);
    shader->use();

    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    va->bind();

    for (unsigned int i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Scene3D::onUpdate(float deltaTime) {
    processKeyboardInput(deltaTime);
    processMouseInput();
}

void Scene3D::onImGuiRender() {
    /*ImGui::Begin("Hello, world!");

    ImGui::Text("camera pos= %f,%f,%f", camera.pos[0],camera.pos[1],camera.pos[2]);
    ImGui::Text("pitch= %f, yaw = %f", camera.pitch, camera.yaw);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();*/
}

void Scene3D::processMouseInput() {

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

void Scene3D::processKeyboardInput(float deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
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