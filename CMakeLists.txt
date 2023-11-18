cmake_minimum_required(VERSION 3.16)
project(audioFrequency)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

include_directories(${PROJECT_SOURCE_DIR}/lib/ ${PROJECT_SOURCE_DIR}/lib/GLFW/include/ ${PROJECT_SOURCE_DIR}/lib/portaudio/)
link_directories(${PROJECT_SOURCE_DIR}/lib/GLFW/lib-mingw ${PROJECT_SOURCE_DIR}/lib/portaudio/lib)

add_executable(audioFrequency main.cpp
        lib/imgui/imgui.cpp lib/imgui/imgui.h
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_impl_glfw.h lib/imgui/imgui_impl_glfw.cpp
        lib/imgui/imgui_impl_opengl3.h lib/imgui/imgui_impl_opengl3.cpp

        lib/stb_image.cpp lib/stb_image.h
        lib/glad.c lib/glad.h

        source/render/core/Shader.cpp source/render/core/Shader.hpp
        source/render/core/VertexBuffer.cpp source/render/core/VertexBuffer.h
        source/render/core/IndexBuffer.cpp source/render/core/IndexBuffer.h
        source/render/core/VertexArray.cpp source/render/core/VertexArray.h
        source/render/core/VertexBufferLayout.cpp source/render/core/VertexBufferLayout.h
        source/render/Renderer.cpp source/render/Renderer.h
        source/render/core/Texture.cpp source/render/core/Texture.h
        source/render/scene/Scene.cpp source/render/scene/Scene.h
        source/render/scene/SceneTriangle.cpp source/render/scene/SceneTriangle.h
        source/render/Configuration.h source/render/Configuration.cpp
        source/render/shapes/Shape.cpp source/render/shapes/Shape.h
        source/render/shapes/Axes3D.cpp source/render/shapes/Axes3D.h source/render/scene/SceneFrequency.cpp source/render/scene/SceneFrequency.h
        source/logic/Audio.cpp source/logic/Audio.h)

target_link_libraries(audioFrequency glfw3 gdi32 opengl32 pthread portaudio)

add_custom_command(TARGET audioFrequency PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:audioFrequency>)

add_custom_command(TARGET audioFrequency PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resource $<TARGET_FILE_DIR:audioFrequency>)

add_custom_command(TARGET audioFrequency PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/lib/portaudio/lib $<TARGET_FILE_DIR:audioFrequency>)

