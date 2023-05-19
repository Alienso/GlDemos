//
// Created by Alienso on 23.9.2022..
//

#ifndef GRASSTERRAIN_CAMERA_H
#define GRASSTERRAIN_CAMERA_H

#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

class Camera {

public:
    Camera();

    bool firstMouse;
    glm::vec3 pos;
    glm::vec3 lastPos;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    glm::mat4x4 transformationMatrix = glm::identity<glm::mat4x4>();
};


#endif //GRASSTERRAIN_CAMERA_H
