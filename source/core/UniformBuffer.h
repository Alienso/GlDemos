//
// Created by Alienso on 14.4.2023..
//

#ifndef GRASSTERRAIN_UNIFORMBUFFER_H
#define GRASSTERRAIN_UNIFORMBUFFER_H


class UniformBuffer {
public:
    UniformBuffer();
    UniformBuffer(const void *data, unsigned int size);
    ~UniformBuffer();
    void bind() const;
    void unbind() const;

private:
    unsigned int rendererID;
};


#endif //GRASSTERRAIN_UNIFORMBUFFER_H
