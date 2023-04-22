//
// Created by Alienso on 23.9.2022..
//

#ifndef GRASSTERRAIN_SCENERAYTRACING_H
#define GRASSTERRAIN_SCENERAYTRACING_H


#include "Scene.h"

class SceneRayTracing : public Scene {
public:
    SceneRayTracing();
    ~SceneRayTracing();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    Shader* shader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
};


#endif //GRASSTERRAIN_SCENERAYTRACING_H
