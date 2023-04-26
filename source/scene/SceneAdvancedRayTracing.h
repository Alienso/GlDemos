//
// Created by Alienso on 12.4.2023..
//

#ifndef GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H
#define GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H


#include "Scene.h"
#include "../core/UniformBuffer.h"
#include "../util/Structs.h"
#include "../core/FrameBuffer.h"
#include "../Camera.h"
#include "../core/ShaderStorageBuffer.h"
#include "../core/Mesh.h"

class SceneAdvancedRayTracing : public Scene {
public:
    explicit SceneAdvancedRayTracing(GLFWwindow* _window);
    ~SceneAdvancedRayTracing();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    GLFWwindow* window;
    Camera camera;

    Shader* shader;
    Shader* screenShader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
    FrameBuffer* fb;
    ShaderStorageBuffer* ssb;

    Mesh* mesh;

    std::vector<Sphere> spheres{};
    Sphere* selectedSphere = nullptr;
    Triangle* selectedTriangle = nullptr;

    std::vector<MeshInfo> meshArray;

    unsigned int framesRendered = 0;
    int resetFrameBuffer = 0;

    void createWidget(bool valueChanged);
    void setupSpheresBasic();
    void setupSpheresReflectingOrbs();
    void setupSpheresReflectingWalls();
    void setupModel();
    void SetUniformSpheres(const std::string& name, std::vector<Sphere>& array);

    void processMouseClick();
    void processMouseInput();
    void processKeyboardInput(float deltaTime);
    void trace(double xPos, double yPos);
    float rayTriangle(glm::vec3& rayOrigin, glm::vec3& rayDir, Triangle tri);
    float raySphere(glm::vec3& rayOrigin, glm::vec3& rayDir, glm::vec3& sphereCenter, float sphereRadius);

    void setUniformMeshInfo(std::vector<MeshInfo>& meshInfo);
    void setUniformTriangles(std::vector<Triangle>& triangles);
};

#endif //GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H
