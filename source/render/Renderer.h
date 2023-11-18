//
// Created by Alienso on 19.9.2022..
//

#ifndef GRASSTERRAIN_RENDERER_H
#define GRASSTERRAIN_RENDERER_H


#include "core/VertexArray.h"
#include "core/Shader.hpp"

class Renderer {

public:
    Renderer() = default;

    void draw(VertexArray& va,IndexBuffer& ib,Shader& shader);
    void clear();
};


#endif //GRASSTERRAIN_RENDERER_H
