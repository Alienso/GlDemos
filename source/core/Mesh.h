//
// Created by Alienson on 25.4.2023..
//

#ifndef GRASSTERRAIN_MESH_H
#define GRASSTERRAIN_MESH_H

#include "glm/glm.hpp"
#include "../scene/rayTracing/structs.h"
#include <vector>

class Mesh {

public:

    Mesh(){}
    Mesh(const char* path);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> verticesNormals;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<unsigned int> indices;
    std::vector<Triangle> triangles;

    Mesh* translate(float x, float y, float z);
    Mesh* translate(glm::vec3 vec);

    Mesh* scale(float scale);
    Mesh* scale(float x, float y, float z);
    Mesh* scale(glm::vec3 scale);

    Mesh* rotate(float x, float y, float z);
    Mesh* rotate(glm::vec3 rotation);

    Mesh* transform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotation);
    Mesh* transform(glm::vec3 translate, float scale, glm::vec3 rotation);

    float* getVerticesData();
    float* getVerticesAndNormalsData();
};


#endif //GRASSTERRAIN_MESH_H
