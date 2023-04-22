//
// Created by Alienso on 19.9.2022..
//

#include "Texture.h"

#include "stb_image.h"
#include "glad.h"

#include <iostream>

Texture::Texture() {
}

Texture::Texture(const char *path, unsigned int type) : rendererID(0), filePath(path), localBuffer(nullptr), width(0), height(0), BPP(0) {

    stbi_set_flip_vertically_on_load(1);
    glGenTextures(1, &rendererID);
    glBindTexture(GL_TEXTURE_2D, rendererID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int nChannels = type == GL_RGB ? 3 : 4;
    unsigned char* data = stbi_load(path, &width, &height, &BPP, nChannels);
    if (data){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D,0);
        //glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else{
        std::cout << "Failed to load texture\n";
    }
}

Texture::~Texture() {
    glDeleteTextures(1,&rendererID);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D,rendererID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D,0);
}
