//
// Created by Alienso on 26.9.2022..
//

#ifndef GRASSTERRAIN_SHAPE_H
#define GRASSTERRAIN_SHAPE_H


#include "../core/VertexArray.h"
#include "../core/Shader.hpp"
#include "../Renderer.h"

class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default;
    virtual void render(glm::mat4& projection,glm::mat4& view, glm::mat4& model) = 0;

protected:
    Renderer renderer;
    VertexBuffer* vb;
    IndexBuffer* ib;
    VertexArray* va;
    VertexBufferLayout* layout;
    Shader* shader = new Shader("basic3D.vs","basic3D.fs");
};


#endif //GRASSTERRAIN_SHAPE_H
