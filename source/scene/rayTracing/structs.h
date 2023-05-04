//
// Created by Alienso on 14.4.2023..
//

#ifndef GRASSTERRAIN_STRUCTS_H
#define GRASSTERRAIN_STRUCTS_H

#include "glm/glm.hpp"

struct RayTracingMaterial{
    glm::vec4 color{};
    glm::vec3 emissionColor{};
    float emissionStrength{};
    float smoothness{};
    float specularProbability{};
    int isInvisibleLightSource{};
    float transparency{};
    float refractionIndex = 1;

    RayTracingMaterial()= default;
    RayTracingMaterial(glm::vec4 _color, glm::vec3 _emissionColor, float _emissionStrength = 0, float _smoothness = 0, float _specularProbability = 0, int _invisible = 0, float _transparency = 0, float _refractionIndex = 1)
    : color(_color),emissionColor(_emissionColor), emissionStrength(_emissionStrength), smoothness(_smoothness), specularProbability(_specularProbability), isInvisibleLightSource(_invisible), transparency(_transparency), refractionIndex(_refractionIndex) {};
};

struct Sphere{
    glm::vec3 position{};
    float radius = 1;
    RayTracingMaterial material;

    Sphere()= default;
    Sphere(glm::vec3 _position, float _radius, RayTracingMaterial _material) : position(_position), radius(_radius), material(_material){}
};

struct Triangle{ //openGL packs vec3's as vec4's so we need the same memory layout here
    alignas(16) glm::vec3 posA{}, posB{}, posC{};
    alignas(16) glm::vec3 normalA{}, normalB{}, normalC{};

    Triangle()= default;
    Triangle(glm::vec3 _posA,glm::vec3 _posB, glm::vec3 _posC, glm::vec3 _normalA, glm::vec3 _normalB, glm::vec3 _normalC)
    : posA(_posA), posB(_posB), posC(_posC), normalA(_normalA), normalB(_normalB), normalC(_normalC){};
};

struct MeshInfo{
    int firstTriangleIndex{};
    int numTriangles{};
    RayTracingMaterial material;
    glm::vec3 boundsMin{};
    glm::vec3 boundsMax{};

    MeshInfo()= default;
    MeshInfo(int _firstTriangleIndex, int _numTriangles, RayTracingMaterial _rayTracingMaterial, glm::vec3 _boundsMin, glm::vec3 _boundsMax) :
    firstTriangleIndex(_firstTriangleIndex), numTriangles(_numTriangles), material(_rayTracingMaterial), boundsMin(_boundsMin), boundsMax(_boundsMax) {}
};


#endif //GRASSTERRAIN_STRUCTS_H
