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
    SceneAdvancedRayTracing();
    ~SceneAdvancedRayTracing();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    Shader* shader;
    Shader* screenShader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
    FrameBuffer* fb;
    Sphere Spheres[10];

    unsigned int framesRendered = 0;
    int resetFrameBuffer = 0;
    float sunX = -60,sunY = 40,sunZ = 5;
    float sunStrength = 1;

    void createWidget(bool valueChanged);
    void setupSpheres();
    void SetUniformSpheres(const std::string& name, Sphere* array, int size);
};

#endif //GRASSTERRAIN_SCENEADVANCEDRAYTRACING_H
