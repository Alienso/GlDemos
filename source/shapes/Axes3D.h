//
// Created by Alienson on 26.9.2022..
//

#ifndef GRASSTERRAIN_AXES3D_H
#define GRASSTERRAIN_AXES3D_H


#include "Shape.h"

class Axes3D : public Shape {

public:
    Axes3D();
    ~Axes3D();
    void render(glm::mat4& projection,glm::mat4& view, glm::mat4& model) override;
};


#endif //GRASSTERRAIN_AXES3D_H
