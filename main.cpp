#include "lib/glad.h"
#include <GLFW/glfw3.h>

#include "lib/glm/glm.hpp"
#include "lib/glm//gtc/matrix_transform.hpp"
#include "lib/glm//gtc/type_ptr.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>

#include "lib/imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "source/scene/SceneTexture.h"
#include "source/scene/SceneTriangle.h"
#include "source/scene/SceneRayTracing.h"
#include "source/Configuration.h"
#include "source/scene/Scene3D.h"
#include "source/scene/SceneRandomWorldGen.h"

void GLAPIENTRY glErrorCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    if (type != GL_DEBUG_TYPE_ERROR){
        return;
    }
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    Configuration::wWidth = width;
    Configuration::wHeight = height;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Configuration::wWidth,Configuration::wHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();

    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( glErrorCallback, 0 );

    srand(glfwGetTime());

    //SceneTexture scene;
    //SceneTriangle scene;
    //SceneRayTracing scene;
    //Scene3D scene(window);
    SceneRandomWorldGen scene(window);

    //printf("Hello!");

    float lastTime = glfwGetTime();
    {
        while (!glfwWindowShouldClose(window)) {

            glfwPollEvents();
            scene.onUpdate(glfwGetTime() - lastTime);
            lastTime = glfwGetTime();

            scene.onRender();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            scene.onImGuiRender();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}