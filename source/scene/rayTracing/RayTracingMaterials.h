//
// Created by Alienso on 14.4.2023..
//

#ifndef GRASSTERRAIN_RAYTRACINGMATERIALS_H
#define GRASSTERRAIN_RAYTRACINGMATERIALS_H

#include "../SceneAdvancedRayTracing.h"

class RayTracingMaterials {
public:

    RayTracingMaterials(){};
    static void init();
    static RayTracingMaterial red;
    static RayTracingMaterial blue;
    static RayTracingMaterial green;
    static RayTracingMaterial white;
    static RayTracingMaterial lightBlue;
    static RayTracingMaterial darkGrey;
    static RayTracingMaterial purple;
    static RayTracingMaterial yellow;
    static RayTracingMaterial cyan;
};


#endif //GRASSTERRAIN_RAYTRACINGMATERIALS_H
