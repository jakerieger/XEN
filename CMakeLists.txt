cmake_minimum_required(VERSION 3.25)

project(GLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Core
        Vendor
)

# Comment out this line to disable OpenGL debug output
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG_GRAPHICS")

find_package(glfw3 3.3 REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(msgpack-cxx CONFIG REQUIRED)
add_library(GLEngine-Core STATIC Core/Shader.cpp Vendor/glad.c
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
        Core/TextRenderer.h
        Core/GraphicsError.cpp
        Core/GraphicsError.h
        Core/FullscreenQuad.cpp
        Core/FullscreenQuad.h
        Core/Shaders/ShaderImports.inl
        Core/PostProcessing.cpp
        Core/PostProcessing.h
        Core/PerformanceMonitor.cpp
        Core/PerformanceMonitor.h)
target_link_libraries(GLEngine-Core PRIVATE glfw assimp::assimp EASTL fmt::fmt Freetype::Freetype msgpack-cxx)

add_executable(DemoApp Demo.cpp)
target_link_libraries(DemoApp PRIVATE GLEngine-Core EASTL)

find_package(Catch2 3 REQUIRED)
add_executable(Tests Tests/test_color.cpp)
target_link_libraries(Tests PRIVATE GLEngine-Core EASTL Catch2::Catch2WithMain)

file(COPY Resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")