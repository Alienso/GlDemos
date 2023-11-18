//
// Created by Alienson on 29.11.2022..
//

#ifndef AUDIOFREQUENCY_SCENEFREQUENCY_H
#define AUDIOFREQUENCY_SCENEFREQUENCY_H


#include "Scene.h"

#include "../../logic/Audio.h"

extern std::vector<double> magnitudes;

class SceneFrequency : public Scene{

public:
    SceneFrequency();
    ~SceneFrequency();
    void onRender() override;
    void onUpdate(float deltaTime) override;
    void onImGuiRender() override;

private:
    int density = 100;
    float scale = 0.5f;
    float* vertices;
    Shader* shader;
    VertexArray* va;
    VertexBuffer* vb;
    VertexBufferLayout* layout;

    void generateVertices(int size);
};


#endif //AUDIOFREQUENCY_SCENEFREQUENCY_H
