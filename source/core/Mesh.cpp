//
// Created by Alienson on 25.4.2023..
//

#include "Mesh.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

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
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                std::cerr << "File can't be read by our simple parser : ( Try exporting with other options\n";
                return;
            }

            indices.push_back(vertexIndex[0] - 1);
            indices.push_back(vertexIndex[1] - 1);
            indices.push_back(vertexIndex[2] - 1);

            triangles.emplace_back(vertices[vertexIndex[0] - 1], vertices[vertexIndex[1] - 1],
                                   vertices[vertexIndex[2] - 1],
                                   verticesNormals[normalIndex[0]], verticesNormals[normalIndex[1]],
                                   verticesNormals[normalIndex[2]]);
        }
    }
}

float* Mesh::getVerticesAndNormalsData() {
    return nullptr;
}

float* Mesh::getVerticesData() {
    return nullptr;
}

Mesh *Mesh::translate(glm::vec3 translate) {
    Mesh* mesh = new Mesh;
    for (auto & vertex : vertices){
        mesh->vertices.emplace_back(vertex.x + translate.x, vertex.y + translate.y, vertex.z + translate.z);
    }
    for (auto & triangle : triangles){
        mesh->triangles.emplace_back(triangle.posA + translate, triangle.posB + translate, triangle.posC + translate,
                                     triangle.normalA, triangle.normalB, triangle.normalC);
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    return mesh;
}

Mesh *Mesh::translate(float x, float y, float z) {
    return translate(glm::vec3(x,y,z));
}

Mesh *Mesh::rotate(glm::vec3 rotation) {
    Mesh* mesh = new Mesh;

    double cosA = cos(glm::radians(rotation.x));
    double cosB = cos(glm::radians(rotation.y));
    double cosY = cos(glm::radians(rotation.z));

    double sinA = sin(glm::radians(rotation.x));
    double sinB = sin(glm::radians(rotation.y));
    double sinY = sin(glm::radians(rotation.z));

    glm::mat3x3 rotationMatrix = { cosA*cosB, cosA*sinB*sinY - sinA*cosY, cosA*sinB*cosY + sinA*sinY,
                                  sinA*cosB, sinA*sinB*sinY + cosA*cosY ,sinA*sinB*cosY - cosA*sinY,
                                  -sinB, cosB*sinY, cosB*cosY};

    for (auto & vertex : vertices){
        mesh->vertices.push_back(vertex * rotationMatrix);
    }
    for (auto & triangle : triangles){
        mesh->triangles.emplace_back(triangle.posA * rotationMatrix, triangle.posB * rotationMatrix, triangle.posC * rotationMatrix,
                                     triangle.normalA, triangle.normalB, triangle.normalC);
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    return mesh;
}

Mesh *Mesh::rotate(float x, float y, float z) {
    return rotate(glm::vec3(x,y,z));
}

Mesh *Mesh::scale(glm::vec3 scale) {
    Mesh* mesh = new Mesh;
    for (auto & vertex : vertices){
        mesh->vertices.emplace_back(vertex.x * scale.x, vertex.y * scale.y, vertex.z * scale.z);
    }
    for (auto & triangle : triangles){
        mesh->triangles.emplace_back(triangle.posA * scale, triangle.posB * scale, triangle.posC * scale,
                                     triangle.normalA, triangle.normalB, triangle.normalC);
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    return mesh;
}

Mesh *Mesh::scale(float x, float y, float z) {
    return scale(glm::vec3(x,y, z));
}

Mesh *Mesh::scale(float s) {
    return scale(glm::vec3(s,s,s));
}

Mesh *Mesh::transform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotation) {
    Mesh* mesh = new Mesh;

    double cosA = cos(glm::radians(rotation.x));
    double cosB = cos(glm::radians(rotation.y));
    double cosY = cos(glm::radians(rotation.z));

    double sinA = sin(glm::radians(rotation.x));
    double sinB = sin(glm::radians(rotation.y));
    double sinY = sin(glm::radians(rotation.z));

    glm::mat3x3 rotationMatrix = { cosA*cosB, cosA*sinB*sinY - sinA*cosY, cosA*sinB*cosY + sinA*sinY,
                                   sinA*cosB, sinA*sinB*sinY + cosA*cosY ,sinA*sinB*cosY - cosA*sinY,
                                   -sinB, cosB*sinY, cosB*cosY};

    for (auto & vertex : vertices){
        glm::vec3 newPos = glm::vec3((vertex.x * scale.x) + translate.x,(vertex.y * scale.y) + translate.y,(vertex.z * scale.z) + translate.z) * rotationMatrix;
        mesh->vertices.push_back(newPos);
    }
    for (auto & triangle : triangles){
        glm::vec3 posA = ((triangle.posA * scale) + translate) * rotationMatrix;
        glm::vec3 posB = ((triangle.posB * scale) + translate) * rotationMatrix;
        glm::vec3 posC = ((triangle.posC * scale) + translate) * rotationMatrix;
        mesh->triangles.emplace_back(posA,posB,posC,
                                     triangle.normalA, triangle.normalB, triangle.normalC);
    }
    mesh->verticesNormals = verticesNormals;
    mesh->textureCoordinates = textureCoordinates;
    mesh->indices = indices;
    return mesh;
}

Mesh *Mesh::transform(glm::vec3 translate, float scale, glm::vec3 rotation) {
    return transform(translate,glm::vec3(scale), rotation);
}
