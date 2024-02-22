cmake_minimum_required(VERSION 3.25)

project(ModelLoading)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(RC_FILE ../res/app.rc)
else ()
    set(RC_FILE ""
    )
endif ()

include_directories(
        ../../Core
)

# Disable deprecation warnings on Windows
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(EASTL CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(ModelLoading main.cpp Demon.cpp ${RC_FILE}
        Floor.cpp
)
target_link_libraries(ModelLoading PRIVATE XEN-Core EASTL fmt::fmt)

file(COPY ../../Resources DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY ../../Sponza DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
