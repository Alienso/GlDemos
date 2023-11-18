//
// Created by Alienson on 20.9.2022..
//

#ifndef GRASSTERRAIN_SCENE_H
#define GRASSTERRAIN_SCENE_H


#include "../Renderer.h"

class Scene {

public:
    Scene() = default;
    ~Scene() = default;

    virtual void onRender() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onImGuiRender() = 0;

protected:
    Renderer renderer;
};


#endif //GRASSTERRAIN_SCENE_H
