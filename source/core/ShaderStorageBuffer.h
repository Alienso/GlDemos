//
// Created by Alienso on 26.4.2023..
//

#ifndef GRASSTERRAIN_SHADERSTORAGEBUFFER_H
#define GRASSTERRAIN_SHADERSTORAGEBUFFER_H


#include "Mesh.h"

class ShaderStorageBuffer {
public:
    ShaderStorageBuffer();
    ShaderStorageBuffer(const void *data, unsigned int size);
    ShaderStorageBuffer(std::vector<Mesh*> &meshes);
    ~ShaderStorageBuffer();

    void bind() const;
    void unbind() const;

    void updateData(std::vector<Mesh *> &meshes);

private:
    unsigned int rendererID;
};


#endif //GRASSTERRAIN_SHADERSTORAGEBUFFER_H
