//
// Created by arsav on 11.11.2022..
//

#ifndef GRASSTERRAIN_SCENEMEMORYALLOCATE_H
#define GRASSTERRAIN_SCENEMEMORYALLOCATE_H

#include "Scene.h"

class SceneMemoryAllocate : public Scene {

public:
    SceneMemoryAllocate(int size);
    ~SceneMemoryAllocate();
    void onRender() override ;
    void onUpdate(float deltaTime) override ;
    void onImGuiRender() override ;

private:
    Shader* shader;
    VertexArray* va;
    VertexBuffer* vb;
    IndexBuffer* ib;
    VertexBufferLayout* layout;
};


#endif //GRASSTERRAIN_SCENEMEMORYALLOCATE_H
