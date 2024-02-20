cmake_minimum_required(VERSION 3.25)

project(XEN)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLES "Build example projects" ON)

include_directories(
        Core
        Vendor
        imgui
        imgui/backends
)

# Comment out this line to disable OpenGL debug output
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG_GRAPHICS")

# Disable deprecation warnings on Windows
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(glfw3 3.3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)

file(GLOB IMGUI_SRCS imgui/*.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_glfw.cpp)

add_library(XEN-Core STATIC
        ${IMGUI_SRCS}
        Vendor/glad.c
        Vendor/stb_image.cpp
        Core/Shader.cpp
        Core/GameApp.cpp
        Core/GameApp.h
        Core/GraphicsContext.cpp
        Core/GraphicsContext.h
        Core/Size.cpp
        Core/Size.h
        Core/Utilities.inl
        Core/Color.cpp
        Core/Color.h
        Core/STL.h
        Core/Resources.h
        Core/STL.cpp
        Core/Input.cpp
        Core/Input.h
        Core/Resources.cpp
        Core/GraphicsError.cpp
        Core/GraphicsError.h
        Core/PostProcessing.cpp
        Core/PostProcessing.h
        Core/Profiler.cpp
        Core/Profiler.h
        Core/Mesh.cpp
        Core/Mesh.h
        Core/Camera.cpp
        Core/Camera.h
        Core/DebugUI.cpp
        Core/DebugUI.h
        Core/Transform.cpp
        Core/Transform.h
        Core/DirectionalLight.cpp
        Core/DirectionalLight.h
        Core/Material.cpp
        Core/Material.h
        Core/Scene.cpp
        Core/Scene.h
        Core/SceneContext.cpp
        Core/SceneContext.h
        Core/Interfaces/GameObject.h
        Core/Interfaces/Component.h
        Core/Interfaces/Lifetime.h
        Core/Interfaces/Drawable.h
        Core/MeshRenderer.cpp
        Core/MeshRenderer.h
        Core/Interfaces/InputListener.h
        Core/InputEvents.h
        Core/InputCodes.h
        Core/Types.h
)
target_link_libraries(XEN-Core PRIVATE glfw assimp::assimp EASTL fmt::fmt msgpack-cxx)

if (BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
    add_executable(Tests Tests/test_gameobject.cpp
            Tests/test_scene.cpp
    )
    target_link_libraries(Tests PRIVATE XEN-Core EASTL Catch2::Catch2WithMain)
endif ()

if (BUILD_EXAMPLES)
    add_subdirectory(Examples/ModelLoading)
endif ()

# Put active project(s) in this file
include(Projects.cmake)