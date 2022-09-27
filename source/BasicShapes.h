//
// Created by Alienson on 24.9.2022..
//

#ifndef GRASSTERRAIN_BASICSHAPES_H
#define GRASSTERRAIN_BASICSHAPES_H


class BasicShapes {

public:
    BasicShapes();
    ~BasicShapes();

    static void axes(float lineWidth);
    static void cube(float size,float r=1.0f,float g=1.0f,float b=1.0f);
    static void sphere(float size,float r=1.0f,float g=1.0f,float b=1.0f);

    static void triangle(float size,float r=1.0f,float g=1.0f,float b=1.0f);
    static void square(float x,float r=1.0f,float g=1.0f,float b=1.0f);
    static void rectangle(float x,float y,float r=1.0f,float g=1.0f,float b=1.0f);
    static void circle(float R,float r=1.0f,float g=1.0f,float b=1.0f);
};


#endif //GRASSTERRAIN_BASICSHAPES_H
