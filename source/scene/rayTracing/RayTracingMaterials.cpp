//
// Created by Alienso on 14.4.2023..
//

#include "RayTracingMaterials.h"

RayTracingMaterial RayTracingMaterials::red;
RayTracingMaterial RayTracingMaterials::green;
RayTracingMaterial RayTracingMaterials::blue;
RayTracingMaterial RayTracingMaterials::white;
RayTracingMaterial RayTracingMaterials::lightBlue;
RayTracingMaterial RayTracingMaterials::darkGrey;
RayTracingMaterial RayTracingMaterials::purple;
RayTracingMaterial RayTracingMaterials::yellow;
RayTracingMaterial RayTracingMaterials::cyan;

void RayTracingMaterials::init(){
    glm::vec4 colorRed = glm::vec4(0.7,0.14,0.14,1.0);
    glm::vec4 colorGreen = glm::vec4(0.0,0.69,0.22,1.0);
    glm::vec4 colorBlue = glm::vec4(0.0,0.41,0.91,1.0);
    glm::vec4 colorWhite = glm::vec4(1.0,1.0,1.0,1.0);
    glm::vec4 colorDarkGrey = glm::vec4(0.2,0.2,0.2,1.0);
    glm::vec4 colorLightBlue = glm::vec4(0.13,0.51,0.95,1.0);
    glm::vec4 colorPurple = glm::vec4(0.35, 0.27,0.5,1.0);
    glm::vec4 colorYellow = glm::vec4(1.0, 0.95,0.2,1.0);
    glm::vec4 colorCyan = glm::vec4(0.0,1.0,1.0,1.0);

    red = {colorRed, colorRed};
    green = {colorGreen, colorGreen};
    blue = {colorBlue, colorBlue};
    white = {colorWhite, colorWhite};
    lightBlue = {colorLightBlue, colorLightBlue};
    darkGrey = {colorDarkGrey, colorDarkGrey};
    purple = {colorPurple, colorPurple};
    yellow = {colorYellow, colorYellow};
    cyan = {colorCyan, colorCyan};
}
