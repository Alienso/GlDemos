//
// Created by Alienso on 19.9.2022..
//

#ifndef GRASSTERRAIN_TEXTURE_H
#define GRASSTERRAIN_TEXTURE_H

#include <string>

class Texture {
private:
    unsigned int rendererID;
    std::string filePath;
    unsigned char* localBuffer;
    int width,height,BPP;
public:
    Texture(const char* path, unsigned int type);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
};


#endif //GRASSTERRAIN_TEXTURE_H
