cmake_minimum_required(VERSION 3.10)
project(3DMaze LANGUAGES CXX)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLUT (works with FreeGLUT)
find_package(GLUT REQUIRED)

add_executable(3DMaze src/main.cpp)

target_include_directories(3DMaze PRIVATE include)

target_link_libraries(3DMaze OpenGL::GL OpenGL::GLU GLUT::GLUT)
