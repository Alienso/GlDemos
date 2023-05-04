//
// Created by Alienso on 19.9.2022..
//

#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() : stride(0) {}

const std::vector<VertexBufferElement> &VertexBufferLayout::getElements() const {
    return elements;
}

unsigned int VertexBufferLayout::getStride() const {
    return stride;
}
