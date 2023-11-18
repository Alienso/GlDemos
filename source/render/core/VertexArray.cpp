//
// Created by Alienso on 19.9.2022..
//

#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1,&renderedID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1,&renderedID);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    this->bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i=0; i<elements.size();i++){
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
        glEnableVertexAttribArray(i);
        offset+=element.count * VertexBufferElement::getSizeOfType(element.type);
    }

}

void VertexArray::bind() const {
    glBindVertexArray(renderedID);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
