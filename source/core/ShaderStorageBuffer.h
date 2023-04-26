//
// Created by Alienson on 26.4.2023..
//

#ifndef GRASSTERRAIN_SHADERSTORAGEBUFFER_H
#define GRASSTERRAIN_SHADERSTORAGEBUFFER_H


class ShaderStorageBuffer {
public:
    ShaderStorageBuffer();
    ShaderStorageBuffer(const void *data, unsigned int size);
    ~ShaderStorageBuffer();

    void bind() const;
    void unbind() const;

private:
    unsigned int rendererID;
};


#endif //GRASSTERRAIN_SHADERSTORAGEBUFFER_H
