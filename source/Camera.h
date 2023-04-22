//
// Created by Alienso on 23.9.2022..
//

#ifndef GRASSTERRAIN_CAMERA_H
#define GRASSTERRAIN_CAMERA_H

#include "../lib/glm/glm.hpp"

class Camera {

public:
    Camera();
    ~Camera();

    bool firstMouse;
    glm::vec3 pos;
    glm::vec3 lastPos;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
};


#endif //GRASSTERRAIN_CAMERA_H
