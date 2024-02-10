cmake_minimum_required(VERSION 3.25)

project(GLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    Classes
    Vendor
)

find_package(glfw3 3.3 REQUIRED)
add_executable(GLEngine GLEngine.cpp Classes/Shader.cpp Vendor/glad.c)
target_link_libraries(GLEngine glfw)

file(COPY Shaders DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")