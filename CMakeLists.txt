cmake_minimum_required(VERSION 3.16)
project(grassTerrain)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/lib/GLFW/include/)
include_directories(${PROJECT_SOURCE_DIR}/lib/)
link_directories(${PROJECT_SOURCE_DIR}/lib/GLFW/lib-mingw)

add_executable(grassTerrain main.cpp
        lib/imgui/imgui.cpp lib/imgui/imgui.h
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_impl_glfw.h lib/imgui/imgui_impl_glfw.cpp
        lib/imgui/imgui_impl_opengl3.h lib/imgui/imgui_impl_opengl3.cpp

        lib/stb_image.cpp lib/stb_image.h
        lib/glad.c lib/glad.h

        source/core/Shader.cpp source/core/Shader.hpp
        source/core/VertexBuffer.cpp source/core/VertexBuffer.h
        source/core/IndexBuffer.cpp source/core/IndexBuffer.h
        source/core/VertexArray.cpp source/core/VertexArray.h
        source/core/VertexBufferLayout.cpp source/core/VertexBufferLayout.h
        source/Renderer.cpp source/Renderer.h
        source/core/Texture.cpp source/core/Texture.h
        source/gen/PerlinNoise.cpp source/gen/PerlinNoise.h
        source/gen/MapGen.cpp source/gen/MapGen.h
        source/scene/Scene.cpp source/scene/Scene.h source/scene/SceneTexture.cpp source/scene/SceneTexture.h source/scene/SceneTriangle.cpp source/scene/SceneTriangle.h source/scene/SceneRayTracing.cpp source/scene/SceneRayTracing.h source/scene/Scene3D.cpp source/scene/Scene3D.h source/Configuration.h source/Camera.cpp source/Camera.h source/Configuration.cpp source/shapes/Circle.cpp source/shapes/Circle.h source/shapes/Shape.cpp source/shapes/Shape.h source/shapes/Axes3D.cpp source/shapes/Axes3D.h source/scene/SceneRandomWorldGen.cpp source/scene/SceneRandomWorldGen.h source/scene/SceneMemoryAllocate.cpp source/scene/SceneMemoryAllocate.h source/scene/SceneAdvancedRayTracing.cpp source/scene/SceneAdvancedRayTracing.h source/core/UniformBuffer.cpp source/core/UniformBuffer.h source/util/RayTracingMaterials.cpp source/util/RayTracingMaterials.h source/util/Structs.h source/core/FrameBuffer.cpp source/core/FrameBuffer.h)

target_link_libraries(grassTerrain glfw3 gdi32 opengl32)

add_custom_command(TARGET grassTerrain PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:grassTerrain>)

add_custom_command(TARGET grassTerrain PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resource $<TARGET_FILE_DIR:grassTerrain>)

