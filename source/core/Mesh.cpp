//
// Created by Alienson on 25.4.2023..
//

#include "Mesh.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>

Mesh::Mesh(const char *path) {
    FILE * file = fopen(path, "r");
    if( file == nullptr ){
        std::cerr<< "Impossible to open the file " << path << '\n';
        return;
    }

    while(true) {
        char lineHeader[128]; //TODO buffer overflow?

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            textureCoordinates.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            verticesNormals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],&normalIndex[2]);
            if (matches != 9) {
                std::cerr << "File can't be read by our simple parser : ( Try exporting with other options\n";
                return;
            }

            indices.push_back(vertexIndex[0] - 1);
            indices.push_back(vertexIndex[1] - 1);
            indices.push_back(vertexIndex[2] - 1);

            triangles.emplace_back(vertices[vertexIndex[0] - 1], vertices[vertexIndex[1] - 1],vertices[vertexIndex[2] - 1],
                                   verticesNormals[normalIndex[0] - 1], verticesNormals[normalIndex[1] - 1],verticesNormals[normalIndex[2] - 1]);
        }
    }
    updateBounds(this);
}

float* Mesh::getVerticesAndNormalsData() {
    return nullptr;
}

float* Mesh::getVerticesData() {
    return nullptr;
}

Mesh *Mesh::translate(glm::vec3& translate, Mesh* mesh) {
    if (mesh == nullptr) {
        mesh = new Mesh;
        mesh->vertices.resize(vertices.size());
        mesh->triangles.resize(triangles.size());
    }

    for (int i=0; i<vertices.size(); i++){
        mesh->vertices[i] = {vertices[i].x + translate.x, vertices[i].y + translate.y, vertices[i].z + translate.z};
    }
    for (int i=0; i<triangles.size(); i++){
        mesh->triangles[i] = {triangles[i].posA + translate, triangles[i].posB + translate, triangles[i].posC + translate,
                              triangles[i].normalA, triangles[i].normalB, triangles[i].normalC};
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    updateBounds(mesh);
    return mesh;
}

Mesh *Mesh::translate(float x, float y, float z, Mesh* mesh) {
    glm::vec3 t = glm::vec3(x,y,z);
    return translate(t,mesh);
}

Mesh *Mesh::rotate(glm::vec3& rotation, Mesh* mesh) {
    if (mesh == nullptr) {
        mesh = new Mesh;
        mesh->vertices.resize(vertices.size());
        mesh->verticesNormals.resize(verticesNormals.size());
        mesh->triangles.resize(triangles.size());
    }

    double cosA = cos(glm::radians(rotation.x));
    double cosB = cos(glm::radians(rotation.y));
    double cosY = cos(glm::radians(rotation.z));

    double sinA = sin(glm::radians(rotation.x));
    double sinB = sin(glm::radians(rotation.y));
    double sinY = sin(glm::radians(rotation.z));

    glm::mat3x3 rotationMatrix = { cosA*cosB, cosA*sinB*sinY - sinA*cosY, cosA*sinB*cosY + sinA*sinY,
                                  sinA*cosB, sinA*sinB*sinY + cosA*cosY ,sinA*sinB*cosY - cosA*sinY,
                                  -sinB, cosB*sinY, cosB*cosY};

    for (int i=0; i<vertices.size(); i++){
        mesh->vertices[i] = vertices[i] * rotationMatrix;
    }
    for (int i=0; i<verticesNormals.size(); i++){
        mesh->verticesNormals[i] = mesh->verticesNormals[i] * rotationMatrix;
    }
    for (int i=0; i<triangles.size(); i++){
        glm::vec3 newNormal = triangles[i].normalA * rotationMatrix;
        mesh->triangles[i] = {triangles[i].posA * rotationMatrix, triangles[i].posB * rotationMatrix, triangles[i].posC * rotationMatrix, newNormal,newNormal,newNormal};
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    updateBounds(mesh);
    return mesh;
}

Mesh *Mesh::rotate(float x, float y, float z, Mesh* mesh) {
    glm::vec3 r = glm::vec3(x,y,z);
    return rotate(r, mesh);
}

Mesh *Mesh::scale(glm::vec3& scale, Mesh* mesh) {
    if (mesh == nullptr) {
        mesh = new Mesh;
        mesh->vertices.resize(vertices.size());
        mesh->triangles.resize(triangles.size());
    }

    for (int i=0; i<vertices.size(); i++){
        mesh->vertices[i] = {vertices[i].x * scale.x, vertices[i].y * scale.y, vertices[i].z * scale.z};
    }
    for (int i=0; i<triangles.size(); i++){
        mesh->triangles[i] = {triangles[i].posA * scale, triangles[i].posB * scale, triangles[i].posC * scale,
                                     triangles[i].normalA, triangles[i].normalB, triangles[i].normalC};
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    updateBounds(mesh);
    return mesh;
}

Mesh *Mesh::scale(float x, float y, float z, Mesh* mesh) {
    glm::vec3 s = glm::vec3(x,y,z);
    return scale(s, mesh);
}

Mesh *Mesh::scale(float s, Mesh* mesh) {
    glm::vec3 sc = glm::vec3(s,s,s);
    return scale(sc, mesh);
}

Mesh *Mesh::transform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotation, Mesh* mesh) {
    if (mesh == nullptr) {
        mesh = new Mesh;
        mesh->vertices.resize(vertices.size());
        mesh->verticesNormals.resize(verticesNormals.size());
        mesh->triangles.resize(triangles.size());
    }

    double cosA = cos(glm::radians(rotation.x));
    double cosB = cos(glm::radians(rotation.y));
    double cosY = cos(glm::radians(rotation.z));

    double sinA = sin(glm::radians(rotation.x));
    double sinB = sin(glm::radians(rotation.y));
    double sinY = sin(glm::radians(rotation.z));

    glm::mat3x3 rotationMatrix = { cosA*cosB, cosA*sinB*sinY - sinA*cosY, cosA*sinB*cosY + sinA*sinY,
                                   sinA*cosB, sinA*sinB*sinY + cosA*cosY ,sinA*sinB*cosY - cosA*sinY,
                                   -sinB, cosB*sinY, cosB*cosY};

    for (int i=0; i<vertices.size(); i++){
        vertices[i] = vertices[i] * rotationMatrix;
        (mesh->vertices)[i] = glm::vec3((vertices[i].x * scale.x) + translate.x,(vertices[i].y * scale.y) + translate.y,(vertices[i].z * scale.z) + translate.z);
    }
    for (int i=0; i<verticesNormals.size(); i++){
        mesh->verticesNormals[i] = mesh->verticesNormals[i] * rotationMatrix;
    }
    for (int i=0; i<triangles.size(); i++){
        glm::vec3 posA = ((triangles[i].posA * rotationMatrix * scale) + translate);
        glm::vec3 posB = ((triangles[i].posB * rotationMatrix * scale) + translate);
        glm::vec3 posC = ((triangles[i].posC * rotationMatrix * scale) + translate);
        glm::vec3 newNormal = triangles[i].normalA * rotationMatrix;
        mesh->triangles[i] = {posA,posB,posC,newNormal,newNormal,newNormal};
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    updateBounds(mesh);
    return mesh;
}

Mesh *Mesh::transform(glm::vec3& translate, float scale, glm::vec3& rotation, Mesh* mesh) {
    glm::vec3 s = glm::vec3(scale);
    return transform(translate, s, rotation, mesh);
}

void Mesh::update() {
    if (m_oldPos != m_pos){
        glm::vec3 diff = m_pos - m_oldPos;
        translate(diff,this);
        m_oldPos = m_pos;
    }
    if (m_oldScale != m_scale){
        glm::vec3 diff = m_scale / m_oldScale;
        scale(diff,this);
        m_oldScale = m_scale;
    }
    if (m_oldRotation != m_rotation){
        glm::vec3 diff = m_rotation - m_oldRotation;
        rotate(diff, this);
        if (m_rotation.x >= 360 || m_rotation.x <= -360)
            m_rotation.x = (int)m_rotation.x % 360;
        if (m_rotation.y >= 360 || m_rotation.y <= -360)
            m_rotation.y = (int)m_rotation.y % 360;
        if (m_rotation.z >= 360 || m_rotation.z <= -360)
            m_rotation.z = (int)m_rotation.z % 360;
        m_oldRotation = m_rotation;
    }
}

void Mesh::updateBounds(Mesh* mesh){
    float floatMin = std::numeric_limits<float>::min();
    float floatMax = std::numeric_limits<float>::max();
    float xMin=floatMax, yMin=floatMax, zMin=floatMax;
    float xMax=floatMin, yMax=floatMin, zMax=floatMin;

    for (auto& vertex : mesh->vertices) {
        if (vertex.x < xMin)
            xMin = vertex.x;
        if (vertex.x > xMax)
            xMax = vertex.x;

        if (vertex.y < yMin)
            yMin = vertex.y;
        if (vertex.y > yMax)
            yMax = vertex.y;

        if (vertex.z < zMin)
            zMin = vertex.z;
        if (vertex.z > zMax)
            zMax = vertex.z;
    }

    mesh->boundsMin = glm::vec3(xMin,yMin,zMin);
    mesh->boundsMax = glm::vec3(xMax,yMax,zMax);
}
