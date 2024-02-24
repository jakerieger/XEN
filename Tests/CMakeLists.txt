cmake_minimum_required(VERSION 3.25)

project(TestSuite)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)
add_executable(Tests
        test_shader.cpp
)
target_link_libraries(Tests PRIVATE XEN-Core EASTL Catch2::Catch2WithMain)