//
// Created by Alienson on 26.9.2022..
//

#ifndef GRASSTERRAIN_CIRCLE_H
#define GRASSTERRAIN_CIRCLE_H


#include "Shape.h"

class Circle : public Shape {
public:
    Circle(float R,float r=1.0f,float g=1.0f,float b=1.0f);
    ~Circle();
    void render(glm::mat4& projection,glm::mat4& view, glm::mat4& model) override;
};


#endif //GRASSTERRAIN_CIRCLE_H
