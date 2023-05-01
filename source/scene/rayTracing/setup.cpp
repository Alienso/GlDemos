//
// Created by Alienson on 28.4.2023..
//

#include <imgui/imgui.h>

#include "../../Configuration.h"
#include "../rayTracing/RayTracingMaterials.h"
#include "../../core/Texture.h"
#include "../../util/Profiler.h"
#include "../SceneAdvancedRayTracing.h"

static void setupMaterialUniforms(const std::string& name, RayTracingMaterial& material, Shader* shader, int i);

void SceneAdvancedRayTracing::setupSpheresBasic() {
    spheres.push_back(new Sphere(glm::vec3(-120, 80, 50), 35.0, RayTracingMaterials::white));
    spheres[0]->material.emissionStrength = 20; //this is the sun
    spheres[0]->material.isInvisibleLightSource = 1;

    spheres.push_back(new Sphere(glm::vec3(16.4,7,-0.65), 10.0, RayTracingMaterials::white));
    spheres.push_back(new Sphere(glm::vec3(-0.9, 3.9, -5), 6.0, RayTracingMaterials::red));
    spheres.push_back(new Sphere(glm::vec3(-12, 2, -4), 4.0, RayTracingMaterials::green));
    spheres.push_back(new Sphere(glm::vec3(-22.5,0.5,-1.3), 3.0, RayTracingMaterials::lightBlue));
    spheres.push_back(new Sphere(glm::vec3(-30.5,-0.5,1), 2.0, RayTracingMaterials::darkGrey));

    spheres.push_back(new Sphere(glm::vec3(0.0, -1002.0, 0.0), 1000, RayTracingMaterials::purple));
}

void SceneAdvancedRayTracing::setupSpheresReflectingOrbs() {
    spheres.push_back(new Sphere(glm::vec3(0.0, 34.0, 0.0), 10, RayTracingMaterials::white));
    spheres[0]->material.emissionStrength = 20; //this is the sun

    spheres.push_back(new Sphere(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::white)); //bottom
    spheres.push_back(new Sphere(glm::vec3(0.0, 1030.0, 0.0), 1000, RayTracingMaterials::white)); //top
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, 1030.0), 1000, RayTracingMaterials::lightBlue)); //back
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, -1030.0), 1000, RayTracingMaterials::darkGrey)); //front
    spheres.push_back(new Sphere(glm::vec3(1030.0, 0, 0.0), 1000, RayTracingMaterials::green)); //right
    spheres.push_back(new Sphere(glm::vec3(-1030.0, 0.0, 0.0), 1000, RayTracingMaterials::red)); //left

    spheres.push_back(new Sphere(glm::vec3(10.0, 0.0, 0.0), 8, RayTracingMaterials::white));
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(-10.0, 0.0, 0.0), 8, RayTracingMaterials::white));
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
}

void SceneAdvancedRayTracing::setupSpheresReflectingWalls() {
    spheres.push_back(new Sphere(glm::vec3(0.0, 34.0, 0.0), 10, RayTracingMaterials::white));
    spheres[0]->material.emissionStrength = 20; //this is the sun

    spheres.push_back(new Sphere(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::white)); //bottom
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(0.0, 1030.0, 0.0), 1000, RayTracingMaterials::white)); //top
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, 1030.0), 1000, RayTracingMaterials::lightBlue)); //back
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, -1030.0), 1000, RayTracingMaterials::darkGrey)); //front
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(1030.0, 0, 0.0), 1000, RayTracingMaterials::green)); //right
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;
    spheres.push_back(new Sphere(glm::vec3(-1030.0, 0.0, 0.0), 1000, RayTracingMaterials::red)); //left
    spheres[spheres.size()-1]->material.smoothness = spheres[spheres.size()-1]->material.specularProbability = 1;

    spheres.push_back(new Sphere(glm::vec3(10.0, 0.0, 0.0), 8, RayTracingMaterials::yellow));
    spheres.push_back(new Sphere(glm::vec3(-10.0, 0.0, 0.0), 8, RayTracingMaterials::cyan));
}

void SceneAdvancedRayTracing::setupModel() {
    spheres.push_back(new Sphere(glm::vec3(-120, 80, 50), 35.0, RayTracingMaterials::white));
    spheres[0]->material.emissionStrength = 10; //this is the sun

    spheres.push_back(new Sphere(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::lightBlue)); //bottom

    Mesh* base = new Mesh("rifle.obj");
    glm::vec3 t = glm::vec3(-10,0,-15);
    glm::vec3 r = glm::vec3(0,90,0);
    Mesh* newMesh = base->transform(t,0.5f,r);
    meshes.push_back(newMesh);
    free(base);

    meshInfoArray.push_back(new MeshInfo(0,(meshes[0])->triangles.size(),RayTracingMaterials::white, meshes[0]->boundsMin, meshes[0]->boundsMax));
}

void SceneAdvancedRayTracing::setupModelIndoors() {
    spheres.push_back(new Sphere(glm::vec3(0.0, 34.0, 0.0), 10, RayTracingMaterials::white));
    spheres[0]->material.emissionStrength = 3; //this is the sun

    spheres.push_back(new Sphere(glm::vec3(0.0, -1030.0, 0.0), 1000, RayTracingMaterials::white)); //bottom
    spheres.push_back(new Sphere(glm::vec3(0.0, 1030.0, 0.0), 1000, RayTracingMaterials::white)); //top
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, 1030.0), 1000, RayTracingMaterials::lightBlue)); //back
    spheres.push_back(new Sphere(glm::vec3(0.0, 0.0, -1030.0), 1000, RayTracingMaterials::white)); //front
    spheres.push_back(new Sphere(glm::vec3(1030.0, 0, 0.0), 1000, RayTracingMaterials::green)); //right
    spheres.push_back(new Sphere(glm::vec3(-1030.0, 0.0, 0.0), 1000, RayTracingMaterials::red)); //left
    spheres.push_back(new Sphere(glm::vec3(0,0,0), 8, RayTracingMaterials::cyan));

    Mesh* base = new Mesh("cube.obj");
    glm::vec3 t = glm::vec3(-0,0,0);
    glm::vec3 r = glm::vec3(20,90,20);
    Mesh* newMesh = base->transform(t,5.0f,r);
    meshes.push_back(newMesh);
    free(base);

    meshInfoArray.push_back(new MeshInfo(0,(meshes[0])->triangles.size(),RayTracingMaterials::white, meshes[0]->boundsMin, meshes[0]->boundsMax));
}

void SceneAdvancedRayTracing::setupCubeRoom() {
    Mesh* base = new Mesh("cube.obj");
    Mesh* light = base->transform(glm::vec3(0,25,0),glm::vec3(5,1,5),glm::vec3(0,0,0));

    Mesh* bottom =  base->transform(glm::vec3(0,-30,0),glm::vec3(30,1,30),glm::vec3(0,0,0));
    Mesh* top =  base->transform(glm::vec3(0,30,0),glm::vec3(30,1,30),glm::vec3(0,0,0));
    Mesh* back =  base->transform(glm::vec3(0,0,30),glm::vec3(30,30,1),glm::vec3(0,0,0));
    Mesh* front =  base->transform(glm::vec3(0,0,-30),glm::vec3(30,30,1),glm::vec3(0,0,0));
    Mesh* right =  base->transform(glm::vec3(30,0,0),glm::vec3(1,30,30),glm::vec3(0,0,0));
    Mesh* left =  base->transform(glm::vec3(-30,0,0),glm::vec3(1,30,30),glm::vec3(0,0,0));

    meshes.push_back(light);
    meshes.push_back(bottom);
    meshes.push_back(top);
    meshes.push_back(back);
    meshes.push_back(front);
    meshes.push_back(right);
    meshes.push_back(left);

    addMeshInfo(light, &RayTracingMaterials::white);
    meshInfoArray[0]->material.emissionStrength = 5;
    addMeshInfo(bottom, &RayTracingMaterials::white);
    addMeshInfo(top, &RayTracingMaterials::white);
    addMeshInfo(back, &RayTracingMaterials::lightBlue);
    addMeshInfo(front, &RayTracingMaterials::white);
    addMeshInfo(right, &RayTracingMaterials::green);
    addMeshInfo(left, &RayTracingMaterials::red);

    free(base);
}

void SceneAdvancedRayTracing::SetUniformSpheres(std::vector<Sphere*>& array){
    PROFILE_SCOPE("SetupUniformSpheres");
    for (int i=0; i<spheres.size(); i++){
        glUniform3f(shader->getUniformLocation("uSpheres[" + std::to_string(i) + "].position"),array[i]->position.x, array[i]->position.y, array[i]->position.z);
        glUniform1f(shader->getUniformLocation("uSpheres[" + std::to_string(i) + "].radius"), array[i]->radius);
        setupMaterialUniforms("uSpheres[",(array[i])->material,shader,i);
    }
}

void SceneAdvancedRayTracing::setUniformMeshInfo(std::vector<MeshInfo*>& meshInfo) {
    PROFILE_SCOPE("SetupUniformMeshInfo");
    for (int i=0; i<meshInfo.size(); i++){
        glUniform1i(shader->getUniformLocation("uMeshInfo[" + std::to_string(i) + "].firstTriangleIndex"), meshInfo[i]->firstTriangleIndex);
        glUniform1i(shader->getUniformLocation("uMeshInfo[" + std::to_string(i) + "].numTriangles"), meshInfo[i]->numTriangles);
        setupMaterialUniforms("uMeshInfo[",meshInfo[i]->material,shader,i);
        glUniform3f(shader->getUniformLocation("uMeshInfo[" + std::to_string(i) + "].boundsMin"), meshInfo[i]->boundsMin.x,meshInfo[i]->boundsMin.y,meshInfo[i]->boundsMin.z);
        glUniform3f(shader->getUniformLocation("uMeshInfo[" + std::to_string(i) + "].boundsMax"), meshInfo[i]->boundsMax.x,meshInfo[i]->boundsMax.y,meshInfo[i]->boundsMax.z);
    }
}

void setupMaterialUniforms(const std::string& name, RayTracingMaterial& material, Shader* shader, int i){
    glUniform4f(shader->getUniformLocation(name + std::to_string(i) + "].material.color"), material.color.x,material.color.y,material.color.z,material.color.w);
    glUniform3f(shader->getUniformLocation(name + std::to_string(i) + "].material.emissionColor"), material.emissionColor.x,material.emissionColor.y,material.emissionColor.z);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.emissionStrength"), material.emissionStrength);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.smoothness"), material.smoothness);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.specularProbability"), material.specularProbability);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.specularProbability"), material.specularProbability);
    glUniform1i(shader->getUniformLocation(name + std::to_string(i) + "].material.isInvisibleLightSource"), material.isInvisibleLightSource);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.transparency"), material.transparency);
    glUniform1f(shader->getUniformLocation(name + std::to_string(i) + "].material.refractionIndex"), material.refractionIndex);

}

/*void SceneAdvancedRayTracing::setUniformTriangles(std::vector<Triangle*>& triangles) {
    for (int i=0; i<triangles.size(); i++){
        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].posA"), triangles[i].posA.x, triangles[i].posA.y, triangles[i].posA.z);
        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].posB"), triangles[i].posB.x, triangles[i].posB.y, triangles[i].posB.z);
        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].posC"), triangles[i].posC.x, triangles[i].posC.y, triangles[i].posC.z);

        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].normalA"), triangles[i].normalA.x, triangles[i].normalA.y, triangles[i].normalA.z);
        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].normalB"), triangles[i].normalB.x, triangles[i].normalB.y, triangles[i].normalB.z);
        glUniform3f(shader->getUniformLocation("uTriangles[" + std::to_string(i) + "].normalC"), triangles[i].normalC.x, triangles[i].normalC.y, triangles[i].normalC.z);
    }
}*/