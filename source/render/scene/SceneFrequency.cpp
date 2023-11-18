//
// Created by Alienson on 29.11.2022..
//

#include "SceneFrequency.h"

SceneFrequency::SceneFrequency() {

    density = Audio::framesPerBuffer;
    int verticesSize = density*12*sizeof(float);

    shader = new Shader("frequency.vs", "frequency.fs");
    vertices = (float*)malloc(verticesSize);
    generateVertices(density);

    vb = new VertexBuffer(vertices, verticesSize);
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
}

void SceneFrequency::generateVertices(int size){
    float step = (2.0f*scale)/size;
    int i = 0;
    for (int j=0;j<size;j++,i+=12){
        vertices[i] = step*j - scale;
        vertices[i+1] = 0.5f;

        vertices[i+2] = step*(j+1) - scale;
        vertices[i+3] = 0.5f;

        vertices[i+4] = step*j - scale;
        vertices[i+5] = 0.0f;

        vertices[i+6] = step*(j+1) - scale;
        vertices[i+7] = 0.5f;

        vertices[i+8] = step*j - scale;
        vertices[i+9] = 0.0f;

        vertices[i+10] = step*(j+1) - scale;
        vertices[i+11] = 0.0f;
    }
}

void SceneFrequency::onRender() {

    glClear(GL_COLOR_BUFFER_BIT);

    for (int i=0,j=0; i<Audio::magnitudes->size(); i++,j+=12){
        float val = fmax( (*Audio::magnitudes)[i] / 100.0 + 0.3, 0);
        vertices[j+1] = val;
        vertices[j+3] = val;
        vertices[j+7] = val;
    }

    /*for (int i=0,j=0; i<density; i++,j+=12){
        float value = rand() % 100 * 1.0 / 200.0;
        vertices[j+1] = value  ;
        vertices[j+3] = value;
        vertices[j+7] = value;
    }*/

    glBufferData(GL_ARRAY_BUFFER,density*12*sizeof(float),vertices,GL_DYNAMIC_DRAW);
    glClearColor(0,0,0,0);

    shader->use();
    va->bind();
    glDrawArrays(GL_TRIANGLES, 0, density*12);
}

void SceneFrequency::onUpdate(float deltaTime) {

}

void SceneFrequency::onImGuiRender() {

}

SceneFrequency::~SceneFrequency() {
    free(shader);
    free(va);
    free(vb);
    free(layout);

    free(vertices);
}
