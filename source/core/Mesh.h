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

    std::vector<glm::vec3> vertices = {};
    std::vector<glm::vec3> verticesNormals = {};
    std::vector<glm::vec2> textureCoordinates = {};
    std::vector<unsigned int> indices = {};
    std::vector<Triangle> triangles = {};
    glm::vec3 m_pos = {0,0,0};
    glm::vec3 m_scale = {1,1,1};
    glm::vec3 m_rotation = {0,0,0};
    glm::vec3 boundsMin = {};
    glm::vec3 boundsMax = {};

    Mesh* translate(float x, float y, float z, Mesh* mesh = nullptr);
    Mesh* translate(glm::vec3& vec, Mesh* mesh = nullptr);

    Mesh* scale(float scale, Mesh* mesh = nullptr);
    Mesh* scale(float x, float y, float z, Mesh* mesh = nullptr);
    Mesh* scale(glm::vec3& scale, Mesh* mesh = nullptr);

    Mesh* rotate(float x, float y, float z, Mesh* mesh = nullptr);
    Mesh* rotate(glm::vec3& rotation, Mesh* mesh = nullptr);

    Mesh* transform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotation, Mesh* mesh = nullptr);
    Mesh* transform(glm::vec3& translate, float scale, glm::vec3& rotation, Mesh* mesh = nullptr);

    float* getVerticesData();
    float* getVerticesAndNormalsData();

    void update();

private:
    void updateBounds(Mesh* mesh);

    glm::vec3 m_oldPos = m_pos;
    glm::vec3 m_oldScale = m_scale;
    glm::vec3 m_oldRotation = m_rotation;
};


#endif //GRASSTERRAIN_MESH_H
