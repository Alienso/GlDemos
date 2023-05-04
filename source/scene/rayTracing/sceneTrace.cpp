//
// Created by Alienson on 28.4.2023..
//

#include "../SceneAdvancedRayTracing.h"
#include "../../Configuration.h"

void SceneAdvancedRayTracing::trace(double xPos, double yPos){
    float minDist = 99999;

    glm::vec2 uv = glm::vec2(xPos,yPos)/glm::vec2(Configuration::wWidth,Configuration::wHeight);

    uv = uv * glm::vec2(2.0) - glm::vec2(1.0);//transform from [0,1] to [-1,1]
    uv.x *= (float)(Configuration::wWidth)/(float)Configuration::wHeight; //aspect fix

    glm::vec3 rayDir = normalize(camera.front + glm::vec3(uv.x,uv.y,0));

    for (auto & sphere : spheres){
        float dist = raySphere(camera.pos, rayDir, sphere->position, sphere->radius);

        if (dist >= 0 && dist < minDist){
            selectedSphere = sphere;
            selectedMesh = nullptr;
            minDist = dist;
        }
    }

    for (int meshIndex = 0; meshIndex < meshInfoArray.size(); meshIndex++) {
        MeshInfo* meshInfo = meshInfoArray[meshIndex];
        //if (!RayBoundingBox(ray, meshInfo.boundsMin, meshInfo.boundsMax)) {
        //    continue;
        //}

        for (int i = 0; i < meshInfo->numTriangles; i++) {
            Triangle* tri = &(meshes[meshIndex]->triangles[i]);
            float dist = rayTriangle(camera.pos, rayDir, tri);

            if (dist >= 0 && dist < minDist) {
                minDist = dist;
                selectedSphere = nullptr;
                selectedMesh = meshes[meshIndex];
                selectedMeshIndex = meshIndex;
            }
        }
    }
}

float SceneAdvancedRayTracing::raySphere(glm::vec3& rayOrigin, glm::vec3& rayDir, glm::vec3& sphereCenter, float sphereRadius){
    float ret = -1;
    glm::vec3 offsetRayOrigin = rayOrigin - sphereCenter;

    float a = dot(rayDir, rayDir);
    float b = 2 * dot(offsetRayOrigin, rayDir);
    float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    //if discriminant is < 0, then we didnt hit the circle
    if (discriminant >= 0){
        float dist = (-b - sqrt(discriminant)) / (2 * a);
        if (dist > 0){
            ret = dist;
        }
    }
    return ret;
}

float SceneAdvancedRayTracing::rayTriangle(glm::vec3& rayOrigin, glm::vec3& rayDir, Triangle* tri){
    glm::vec3 edgeAB = tri->posB - tri->posA;
    glm::vec3 edgeAC = tri->posC - tri->posA;
    glm::vec3 normalVector = cross(edgeAB, edgeAC);
    glm::vec3 ao = rayOrigin - tri->posA;
    glm::vec3 dao = cross(ao, rayDir);

    float determinant = -dot(rayDir, normalVector);
    float invDet = 1 / determinant;

    // Calculate dst to triangle & barycentric coordinates of intersection point
    float dst = dot(ao, normalVector) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;

    // Initialize hit info
    bool didHit = determinant >= 1E-6 && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
    glm::vec3 hitPoint = rayOrigin + rayDir * dst;
    glm::vec3 normal = normalize(tri->normalA * w + tri->normalB * u + tri->normalC * v);
    float dist = dst;
    if (didHit)
        return dist;
    else return 99999;
}
