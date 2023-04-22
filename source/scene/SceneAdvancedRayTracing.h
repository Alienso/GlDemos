//
// Created by Alienso on 12.4.2023..
//

#ifndef GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H
#define GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H


#include "Scene.h"
#include "../core/UniformBuffer.h"
#include "../util/Structs.h"
#include "../core/FrameBuffer.h"

class SceneAdvancedRayTracing : public Scene {
public:
    explicit SceneAdvancedRayTracing(GLFWwindow* _window);
    ~SceneAdvancedRayTracing();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    GLFWwindow* window;

    Shader* shader;
    Shader* screenShader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
    FrameBuffer* fb;
    Sphere spheres[10];
    Sphere* selectedSphere = nullptr;

    unsigned int framesRendered = 0;
    int resetFrameBuffer = 0;
    float sunX = -60,sunY = 40,sunZ = 5;
    float sunStrength = 1;

    void createWidget(bool valueChanged);
    void setupSpheres();
    void SetUniformSpheres(const std::string& name, Sphere* array, int size);

    void processMouseInput();
    void trace(double xPos, double yPos);
    float raySphere(glm::vec3& rayOrigin, glm::vec3& rayDir, glm::vec3& sphereCenter, float sphereRadius);

};

#endif //GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H
