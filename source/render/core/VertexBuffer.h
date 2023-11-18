//
// Created by Alienson on 19.9.2022..
//

#ifndef GRASSTERRAIN_VERTEXBUFFER_H
#define GRASSTERRAIN_VERTEXBUFFER_H


class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;

private:
    unsigned int rendererID;
};


#endif //GRASSTERRAIN_VERTEXBUFFER_H
