//
// Created by Alienso on 19.9.2022..
//

#ifndef GRASSTERRAIN_VERTEXBUFFERLAYOUT_H
#define GRASSTERRAIN_VERTEXBUFFERLAYOUT_H

#include "glad.h"

#include <vector>

struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    static unsigned int getSizeOfType(unsigned int type){
        switch (type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
public:
    unsigned int getStride() const;

public:
    VertexBufferLayout();

    template<typename T>
    void push(unsigned int count){
        if (typeid(T) == typeid(float)){
            elements.push_back({GL_FLOAT,count,GL_FALSE});
            stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
        }
        if (typeid(T) == typeid(unsigned int)){
            elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE});
            stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
        }
        if (typeid(T) == typeid(unsigned char)){
            elements.push_back({GL_UNSIGNED_BYTE,count,GL_TRUE});
            stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
        }
    }

    const std::vector<VertexBufferElement> &getElements() const;
};


#endif //GRASSTERRAIN_VERTEXBUFFERLAYOUT_H
