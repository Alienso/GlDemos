//
// Created by Alienso on 14.4.2023..
//

#include "FrameBuffer.h"
#include "../Configuration.h"

#include <iostream>

#include "glad.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

    createTexture();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

}

FrameBuffer::~FrameBuffer() {
    glDeleteBuffers(1,&rendererID);
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
}

void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture() const {
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
}

void FrameBuffer::createTexture(){
    glGenTextures(1, &renderedTexture);

    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Configuration::wWidth, Configuration::wHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
}