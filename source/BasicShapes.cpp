//
// Created by Alienson on 24.9.2022..
//

#include "BasicShapes.h"

#include "../lib/glad.h"
#include <GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"
#include "core/VertexBuffer.h"

BasicShapes::BasicShapes() {
}

BasicShapes::~BasicShapes() {

}

void BasicShapes::axes(float lineWidth) {
    float oldLineWidth;
    glGetFloatv(GL_LINE_WIDTH,&oldLineWidth);
    glLineWidth(lineWidth);

    float vertices[] = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
    };

    VertexBuffer va(vertices,sizeof(vertices));
    va.bind();
    glDrawArrays(GL_LINES,0,sizeof(vertices));
}

void BasicShapes::cube(float size, float r, float g, float b) {

}

void BasicShapes::sphere(float size, float r, float g, float b) {

}

void BasicShapes::triangle(float size, float r, float g, float b) {

}

void BasicShapes::square(float x, float r, float g, float b) {

}

void BasicShapes::rectangle(float x, float y, float r, float g, float b) {

}

void BasicShapes::circle(float R, float r, float g, float b) {

}
