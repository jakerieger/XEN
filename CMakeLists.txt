cmake_minimum_required(VERSION 3.25)

project(GLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Vendor
)

add_executable(GLEngine main.cpp Core/Shader.cpp Vendor/glad.c
        Vendor/stb_image.cpp
        Core/GameApp.cpp
        Core/GameApp.h
        Core/GraphicsContext.cpp
        Core/GraphicsContext.h
        Core/Size.cpp
        Core/Size.h)

find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
target_link_libraries(GLEngine glfw assimp)

file(COPY Resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")