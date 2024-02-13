cmake_minimum_required(VERSION 3.25)

project(GLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Vendor
)

find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
find_package(EASTL CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
add_executable(GLEngine Demo.cpp Core/Shader.cpp Vendor/glad.c
        Vendor/stb_image.cpp
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
        Core/TextRenderer.cpp
        Core/TextRenderer.h)
target_link_libraries(GLEngine PRIVATE glfw assimp EASTL fmt::fmt Freetype::Freetype)

find_package(Catch2 3 REQUIRED)
add_executable(Tests Tests/test_color.cpp)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

file(COPY Resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")