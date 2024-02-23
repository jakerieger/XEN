cmake_minimum_required(VERSION 3.25)

project(ModelConverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable deprecation warnings on Windows
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(assimp CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

add_executable(ModelConverter main.cpp app.rc)
target_link_libraries(ModelConverter PRIVATE assimp::assimp rapidjson)