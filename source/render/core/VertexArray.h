//
// Created by Alienso on 19.9.2022..
//

#ifndef GRASSTERRAIN_VERTEXARRAY_H
#define GRASSTERRAIN_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {

public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;

private:
    unsigned int renderedID;
};


#endif //GRASSTERRAIN_VERTEXARRAY_H
