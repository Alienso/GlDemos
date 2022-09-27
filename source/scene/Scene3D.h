//
// Created by Alienson on 23.9.2022..
//

#ifndef GRASSTERRAIN_SCENE3D_H
#define GRASSTERRAIN_SCENE3D_H


#include "Scene.h"
#include "../Camera.h"
#include "../core/Texture.h"
#include "../shapes/Axes3D.h"
#include "../../../../../../MinGW/include/GLFW/glfw3.h"

class Scene3D : public Scene {
public:
    Scene3D(GLFWwindow* window);
    ~Scene3D();
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
    Texture* texture1;
    Texture* texture2;
    glm::vec3 cubePositions[10];

    void processMouseInput();
    void processKeyboardInput(float deltaTime);
};

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#endif //GRASSTERRAIN_SCENE3D_H
