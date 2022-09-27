//
// Created by Alienson on 26.9.2022..
//

#ifndef GRASSTERRAIN_SCENERANDOMWORLDGEN_H
#define GRASSTERRAIN_SCENERANDOMWORLDGEN_H


#include "Scene.h"
#include "../Camera.h"
#include "../shapes/Axes3D.h"

class SceneRandomWorldGen : public Scene {
public:
    SceneRandomWorldGen(GLFWwindow* window);
    ~SceneRandomWorldGen();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    Axes3D axes3D;
    Camera camera;
    Shader* shader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
    GLFWwindow* window;

    void processMouseInput();
    void processKeyboardInput(float deltaTime);
};

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif //GRASSTERRAIN_SCENERANDOMWORLDGEN_H
