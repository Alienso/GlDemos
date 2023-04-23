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

    //setupSpheresBasic();
    //setupSpheresReflectingWalls();
    setupSpheresReflectingOrbs();

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

void SceneAdvancedRayTracing::setupSpheresBasic() {
    spheres.emplace_back(glm::vec3(-120, 80, 50), 35.0, RayTracingMaterials::white);
    spheres[0].material.emissionStrength = 10; //this is the sun
    spheres[0].material.isInvisibleLightSource = 1;

    spheres.emplace_back(glm::vec3(16.4,7,-0.65), 10.0, RayTracingMaterials::white);
    spheres.emplace_back(glm::vec3(-0.9, 3.9, -5), 6.0, RayTracingMaterials::red);
    spheres.emplace_back(glm::vec3(-12, 2, -4), 4.0, RayTracingMaterials::green);
    spheres.emplace_back(glm::vec3(-22.5,0.5,-1.3), 3.0, RayTracingMaterials::lightBlue);
    spheres.emplace_back(glm::vec3(-30.5,-0.5,1), 2.0, RayTracingMaterials::darkGrey);

    spheres.emplace_back(glm::vec3(0.0, -1002.0, 0.0), 1000, RayTracingMaterials::purple);
}

void SceneAdvancedRayTracing::setupSpheresReflectingOrbs() {
    spheres.emplace_back(glm::vec3(0.0, 34.0, 0.0), 10, RayTracingMaterials::white);
    spheres[0].material.emissionStrength = 20; //this is the sun

    spheres.emplace_back(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::white); //bottom
    spheres.emplace_back(glm::vec3(0.0, 1030.0, 0.0), 1000, RayTracingMaterials::white); //top
    spheres.emplace_back(glm::vec3(0.0, 0.0, 1030.0), 1000, RayTracingMaterials::lightBlue); //back
    spheres.emplace_back(glm::vec3(0.0, 0.0, -1030.0), 1000, RayTracingMaterials::darkGrey); //front
    spheres.emplace_back(glm::vec3(1030.0, 0, 0.0), 1000, RayTracingMaterials::green); //right
    spheres.emplace_back(glm::vec3(-1030.0, 0.0, 0.0), 1000, RayTracingMaterials::red); //left

    spheres.emplace_back(glm::vec3(10.0, 0.0, 0.0), 8, RayTracingMaterials::white);
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(-10.0, 0.0, 0.0), 8, RayTracingMaterials::white);
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
}

void SceneAdvancedRayTracing::setupSpheresReflectingWalls() {
    spheres.emplace_back(glm::vec3(0.0, 34.0, 0.0), 10, RayTracingMaterials::white);
    spheres[0].material.emissionStrength = 20; //this is the sun

    spheres.emplace_back(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::white); //bottom
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(0.0, 1030.0, 0.0), 1000, RayTracingMaterials::white); //top
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(0.0, 0.0, 1030.0), 1000, RayTracingMaterials::lightBlue); //back
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(0.0, 0.0, -1030.0), 1000, RayTracingMaterials::darkGrey); //front
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(1030.0, 0, 0.0), 1000, RayTracingMaterials::green); //right
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;
    spheres.emplace_back(glm::vec3(-1030.0, 0.0, 0.0), 1000, RayTracingMaterials::red); //left
    spheres[spheres.size()-1].material.smoothness = spheres[spheres.size()-1].material.specularProbability = 1;

    spheres.emplace_back(glm::vec3(10.0, 0.0, 0.0), 8, RayTracingMaterials::yellow);
    spheres.emplace_back(glm::vec3(-10.0, 0.0, 0.0), 8, RayTracingMaterials::cyan);
}

void SceneAdvancedRayTracing::SetUniformSpheres(const std::string& name, std::vector<Sphere>& array){
   for (int i=0; i<spheres.size(); i++){
       glUniform3f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].position"),array[i].position.x, array[i].position.y, array[i].position.z);
       glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].radius"), array[i].radius);

       glUniform4f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.color"), array[i].material.color.x,array[i].material.color.y,array[i].material.color.z,array[i].material.color.w);
       glUniform3f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.emissionColor"), array[i].material.emissionColor.x,array[i].material.emissionColor.y,array[i].material.emissionColor.z);
       glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.emissionStrength"), array[i].material.emissionStrength);
       glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.smoothness"), array[i].material.smoothness);
       glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.specularProbability"), array[i].material.specularProbability);
       glUniform1f(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.specularProbability"), array[i].material.specularProbability);
       glUniform1i(shader->getUniformLocation(name + "[" + std::to_string(i) + "].material.isInvisibleLightSource"), array[i].material.isInvisibleLightSource);
   }
}

void SceneAdvancedRayTracing::onRender() {

   shader->use();
   fb->bind();

   shader->setFloat("uTime", glfwGetTime());
   shader->setVec4("uMouse",0,0,0,1);
   shader->setVec2("uResolution",Configuration::wWidth,Configuration::wHeight);
   shader->setVec3("uCameraPos", camera.pos);
   shader->setVec3("uCameraFocusPoint", camera.front);

   shader->setInt("uSphereCount", spheres.size());
   SetUniformSpheres("uSpheres", spheres);
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
   //processMouseClick(); //todo temp moved to onImGuiRender
   //processMouseInput();
   processKeyboardInput(deltaTime);
   resetFrameBuffer = 0;
}

void SceneAdvancedRayTracing::onImGuiRender() {

   ImGui::Begin("Ray Tracing Demo");

   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   if(selectedSphere != nullptr){
       createWidget(ImGui::SliderFloat("X",&selectedSphere->position.x,-50,50));
       createWidget(ImGui::SliderFloat("Y",&selectedSphere->position.y,-50,50));
       createWidget(ImGui::SliderFloat("Z",&selectedSphere->position.z,-50,50));

       createWidget(ImGui::InputFloat("Radius",&selectedSphere->radius,0.1f,1));
       createWidget(ImGui::ColorEdit3("Color", &selectedSphere->material.color.x));  //TODO this probably shouldn't be done like this
       createWidget(ImGui::ColorEdit3("Emission Color", &selectedSphere->material.emissionColor.x));
       createWidget(ImGui::InputFloat("Emission Strength",&selectedSphere->material.emissionStrength, 0.1f, 1));
       createWidget(ImGui::SliderFloat("Smoothness",&selectedSphere->material.smoothness, 0, 1));
       createWidget(ImGui::SliderFloat("Specular probability",&selectedSphere->material.specularProbability, 0, 1));
       createWidget(ImGui::SliderInt("Invisible light source",&selectedSphere->material.isInvisibleLightSource,0,1));
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

void SceneAdvancedRayTracing::trace(double xPos, double yPos){
   float minDist = 99999;

   glm::vec2 uv = glm::vec2(xPos,yPos)/glm::vec2(Configuration::wWidth,Configuration::wHeight);

   uv = uv * glm::vec2(2.0) - glm::vec2(1.0);//transform from [0,1] to [-1,1]
   uv.x *= (float)(Configuration::wWidth)/(float)Configuration::wHeight; //aspect fix

   glm::vec3 rayDir = normalize(camera.front + glm::vec3(uv.x,uv.y,0));

   for (int i=0; i< spheres.size(); i++){
       float dist = raySphere(camera.pos, rayDir, spheres[i].position, spheres[i].radius);

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