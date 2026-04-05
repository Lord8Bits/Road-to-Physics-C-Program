# 1. Setup
cmake_minimum_required(VERSION 3.28)
project(Road-to-Physics-C-Program CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
# -----------------------------------------------------------------------------
# 3. Create Targets (Executables)
# We map a unique Executable Name to the source files in your subdirectories.
# -----------------------------------------------------------------------------

# --- 1D Program ---
add_executable(Physics1D 1D/main.cpp)

# --- 2D Program ---
add_executable(Physics2D 2D/main.cpp 2D/glad.c)
target_compile_options(Physics2D PRIVATE -Wall -O2)

# --- 3D Program ---
add_executable(Physics3D 3D/main.cpp)

# -----------------------------------------------------------------------------
# 4. Linking Libraries (Crucial for Physics)
# On Linux/macOS, you often need to explicitly link the math library (-lm).
# -----------------------------------------------------------------------------

if(UNIX)
    target_link_libraries(Physics1D m)
endif()

target_link_libraries(Physics2D PRIVATE OpenGL::GL glfw)
target_include_directories(Physics2D PRIVATE 2D/include)