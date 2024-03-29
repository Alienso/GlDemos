//
// Created by Alienso on 19.9.2022..
//

#include "Renderer.h"

void Renderer::draw(VertexArray &va,IndexBuffer &ib, Shader &shader) {
    shader.use();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}
