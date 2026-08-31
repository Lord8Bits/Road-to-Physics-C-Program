# 1. Setup
cmake_minimum_required(VERSION 3.28)
project(Road-to-Physics-C-Program CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# --- 1D Program ---
add_executable(Physics1D 1D/main.c)

# --- 2D Program ---
file(GLOB SOURCE_FILES "2D/*.cpp")
add_executable(Physics2D
        ${SOURCE_FILES}
        2D/include/glad/glad.c)

target_compile_options(Physics2D PRIVATE -Wall -O2)

# --- 3D Program ---
add_executable(Physics3D 3D/main.cpp)


if(UNIX)
    target_link_libraries(Physics1D m)
endif()

target_link_libraries(Physics2D PRIVATE OpenGL::GL glfw)