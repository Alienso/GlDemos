//
// Created by Alienso on 14.4.2023..
//

#include "RayTracingMaterials.h"

RayTracingMaterial RayTracingMaterials::red;
RayTracingMaterial RayTracingMaterials::green;
RayTracingMaterial RayTracingMaterials::blue;
RayTracingMaterial RayTracingMaterials::light;

void RayTracingMaterials::init(){
    glm::vec4 colorRed = glm::vec4(0.7,0.14,0.14,1.0);
    glm::vec4 colorGreen = glm::vec4(0.0,0.69,0.22,1.0);
    glm::vec4 colorBlue = glm::vec4(0.0,0.41,0.91,1.0);
    glm::vec4 colorWhite = glm::vec4(1.0,1.0,1.0,1.0);

    glm::vec3 emissionWhite = glm::vec3(1.0,1.0,1.0);

    red = {colorRed, emissionWhite, 0};
    green = {colorGreen, emissionWhite, 0};
    blue = {colorBlue, emissionWhite, 0};
    light = {colorWhite, emissionWhite, 0};
}
