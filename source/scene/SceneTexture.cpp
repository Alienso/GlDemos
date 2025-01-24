//
// Created by Alienso on 20.9.2022..
//

#include <imgui/imgui.h>
#include "SceneTexture.h"

SceneTexture::SceneTexture(){

    shader = new Shader("texture.vs","texture.fs");
    float vertices[] = {
            -1.0f, -1.0f,0.0f, 0.0f, // bottom right
            1.0f, -1.0f, 1.0f, 0.0f, // bottom left
            1.0f, 1.0f, 1.0f, 1.0f, // top
            -1.0f, 1.0f, 0.0f, 1.0f //bottom
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    vb = new VertexBuffer(vertices, sizeof(vertices));
    layout = new VertexBufferLayout();
    layout->push<float>(2);
    layout->push<float>(2);
    va = new VertexArray();
    va->addBuffer(*vb, *layout);
    ib = new IndexBuffer(indices, 6);

    texture = new Texture ("textures/10.jpg",GL_RGB);
    texture->bind();
    shader->use();
}

SceneTexture::~SceneTexture(){
    free(shader);
    free(va);
    free(vb);
    free(layout);
    free(ib);
    free(texture);
};

void SceneTexture::onRender() {
    shader->setVec4("u_Color",0.8f,0.3f,0.8f,1.0f);
    shader->setInt("u_Texture",0);
    renderer.draw(*va,*ib,*shader);
}

void SceneTexture::onUpdate(float deltaTime) {
}

void SceneTexture::onImGuiRender() {
    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    /*if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);*/

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
