//
// Created by Alienso on 23.9.2022..
//

#ifndef GRASSTERRAIN_SCENETRIANGLE_H
#define GRASSTERRAIN_SCENETRIANGLE_H


#include "Scene.h"

class SceneTriangle : public Scene {

public:
    SceneTriangle();
    ~SceneTriangle();
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


#endif //GRASSTERRAIN_SCENETRIANGLE_H
