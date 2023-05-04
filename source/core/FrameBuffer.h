//
// Created by Alienso on 14.4.2023..
//

#ifndef GRASSTERRAIN_FRAMEBUFFER_H
#define GRASSTERRAIN_FRAMEBUFFER_H


class FrameBuffer {
public:
    FrameBuffer();
    ~FrameBuffer();
    void bind() const;
    void unbind() const;
    void bindTexture() const;
    void createTexture();

private:
    unsigned int rendererID;
    unsigned int renderedTexture;
};


#endif //GRASSTERRAIN_FRAMEBUFFER_H
