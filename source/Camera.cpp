//
// Created by Alienso on 23.9.2022..
//

#include "Camera.h"
#include "Configuration.h"

Camera::Camera() : firstMouse(true), yaw(-90.0f), pitch(0.0f), pos(0.0f, 0.0f, 3.0f), lastPos(Configuration::wWidth/2,Configuration::wHeight/2,3.0f), front(0.0f, 0.0f, -1.0f),up(0.0f, 1.0f, 0.0f) {

}
