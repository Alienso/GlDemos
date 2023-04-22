//
// Created by Alienso on 14.4.2023..
//

#ifndef GRASSTERRAIN_STRUCTS_H
#define GRASSTERRAIN_STRUCTS_H

#include "glm/glm.hpp"

struct RayTracingMaterial{
    glm::vec4 color;
    glm::vec3 emissionColor;
    float emissionStrength;
    float smoothness;

    RayTracingMaterial(){};
    RayTracingMaterial(glm::vec4 _color, glm::vec3 _emissionColor, float _emissionStrength, float _smoothness) : color(_color),
    emissionColor(_emissionColor), emissionStrength(_emissionStrength), smoothness(_smoothness){};
};

struct Sphere{
    glm::vec3 position;
    float radius;
    RayTracingMaterial material;

    Sphere(){}
    Sphere(glm::vec3 _position, float _radius, RayTracingMaterial _material) : position(_position), radius(_radius), material(_material){}
};


#endif //GRASSTERRAIN_STRUCTS_H
