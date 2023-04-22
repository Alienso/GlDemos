//
// Created by Alienso on 26.9.2022..
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

    int width = 500,height = 500;
    mapGen = new MapGen(width,height,0.1,4,0.5f,2,1);

    setupTerrain();
    setupGrass();
}

void SceneRandomWorldGen::setupTerrain() {
    terrainShader = new Shader("perlin.vs", "perlin.fs");

    int width = mapGen->getWidth();
    int height = mapGen->getHeight();
    float* vertices = (float*)calloc(width*height,sizeof(float)*6);
    unsigned int* indices = (unsigned int*)calloc((width-1)*(height-1),2*3*sizeof(unsigned int)); //2 triangles per square *3 vertexes per triangle

    mapGen->generateMap(&vertices,&indices);

    terrainVb = new VertexBuffer(vertices, width * height * sizeof(float) * 6);
    terrainLayout = new VertexBufferLayout();
    terrainLayout->push<float>(3);
    terrainLayout->push<float>(3);
    terrainVa = new VertexArray();
    terrainVa->addBuffer(*terrainVb, *terrainLayout);
    terrainIb = new IndexBuffer(indices, (width - 1) * (height - 1) * 2 * 3);

    free(vertices);
    free(indices);
}

void SceneRandomWorldGen::setupGrass() {

    grassShader = new Shader("grass.vs", "grass.fs");

    int density = 1;
    int width = mapGen->getWidth();
    int height = mapGen->getHeight();
    int verticesSize = (width*height*density * 2 - 1)*7*7;
    float* vertices = (float*)calloc(verticesSize,sizeof(float)); //each grass has 13 vertices
    unsigned int* indices = (unsigned int*)calloc(width*height*density,5*4*3*sizeof(unsigned int));

    mapGen->generateGrass(vertices,indices,density);

    grassVb = new VertexBuffer(vertices, verticesSize*sizeof(float));
    grassLayout = new VertexBufferLayout();
    grassLayout->push<float>(3);
    grassLayout->push<float>(3);
    grassLayout->push<float>(1);
    grassVa = new VertexArray();
    grassVa->addBuffer(*grassVb, *grassLayout);
    grassIb = new IndexBuffer(indices, width*height*density * 5*4*3);
}

void SceneRandomWorldGen::onRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(Configuration::fov), (float)Configuration::wWidth/(float)Configuration::wHeight,
                                            0.01f, 1000.0f);
    glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
    glm::mat4 model = glm::mat4(1.0f);

    axes3D.render(projection,view,model);

    terrainShader->use();
    terrainShader->setMat4("projection", projection);
    terrainShader->setMat4("view", view);
    terrainShader->setMat4("model",model);
    terrainShader->setVec3("uColor",0.0f,1.0,0.0f);
    renderer.draw(*terrainVa, *terrainIb, *terrainShader);

    grassShader->use();
    grassShader->setMat4("projection", projection);
    grassShader->setMat4("view", view);
    grassShader->setMat4("model",model);
    grassShader->setVec3("uColor",0.0f,1.0,0.0f);
    grassShader->setFloat("uTime",glfwGetTime());
    renderer.draw(*grassVa, *grassIb, *grassShader);
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

SceneRandomWorldGen::~SceneRandomWorldGen() {
    free(terrainShader);
    free(terrainVa);
    free(terrainVb);
    free(terrainLayout);
    free(terrainIb);

    free(grassShader);
    free(grassVa);
    free(grassVb);
    free(grassLayout);
    free(grassIb);

    free(mapGen);
    glDisable(GL_DEPTH_TEST);
}
