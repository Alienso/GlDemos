//
// Created by Alienson on 19.9.2022..
//

#ifndef GRASSTERRAIN_INDEXBUFFER_H
#define GRASSTERRAIN_INDEXBUFFER_H


class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;

private:
    unsigned int rendererID;
    unsigned int count;
public:
    inline unsigned int getCount() const{
        return count;
    }
};


#endif //GRASSTERRAIN_INDEXBUFFER_H
