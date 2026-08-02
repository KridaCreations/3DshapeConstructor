cmake_minimum_required(VERSION 4.3)
project(ShapeReconstructor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

# Executable
add_executable(OpenGLProject
        source/main.cpp
        source/glad.c
        include/Shader.h
        source/stb_image.cpp
        source/DrawingElement.cpp
        include/DrawingElement.h
        source/VoxelManager.cpp
        include/VoxelManager.h
        Cube.cpp
        Cube.h
)

# Include directories
target_include_directories(OpenGLProject PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(OpenGLProject PRIVATE
        OpenGL::GL
        glfw
        GLEW::GLEW
        glm::glm
)