//
// Created by Alienso on 20.9.2022..
//

#ifndef GRASSTERRAIN_SCENETEXTURE_H
#define GRASSTERRAIN_SCENETEXTURE_H


#include "Scene.h"
#include "../core/Shader.hpp"
#include "../core/VertexArray.h"
#include "../core/Texture.h"

class SceneTexture : public Scene {

public:

    SceneTexture();
    ~SceneTexture();
    void onRender() override;
    void onUpdate(float deltaTime) override;
    void onImGuiRender() override;

private:
    Shader* shader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;
    IndexBuffer* ib;
    Texture* texture;
};


#endif //GRASSTERRAIN_SCENETEXTURE_H
