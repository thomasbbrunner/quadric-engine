
# Project
cmake_minimum_required(VERSION 3.2)
project(quadric-engine)

# CMake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O1") # -g -Og

# Executables
message(STATUS "Looking for main file in ${PROJECT_SOURCE_DIR}")
add_executable(qe
    ${PROJECT_SOURCE_DIR}/qe/api.cpp
    ${PROJECT_SOURCE_DIR}/qe/buffer.cpp
    ${PROJECT_SOURCE_DIR}/qe/camera.cpp
    ${PROJECT_SOURCE_DIR}/qe/math.cpp
    ${PROJECT_SOURCE_DIR}/qe/print.cpp
    ${PROJECT_SOURCE_DIR}/qe/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/third_party/glad/glad.c)

# Headers
include_directories(${PROJECT_SOURCE_DIR}/qe)
include_directories(${PROJECT_SOURCE_DIR}/third_party)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND) # forcing use of GLVND libs instead of legacy
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL in ${OPENGL_gl_LIBRARY}")
target_link_libraries(qe ${OPENGL_gl_LIBRARY})

# GLM
find_package(glm REQUIRED)
message(STATUS "Found glm in ${glm_DIR}")
target_link_libraries(qe glm::glm)

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found glfw3 in ${glfw3_DIR}")
target_link_libraries(qe glfw) 

# Eigen
find_package(Eigen3 REQUIRED)
message(STATUS "Found Eigen in ${Eigen3_DIR}")
target_link_libraries(qe Eigen3::Eigen)
include_directories(${EIGEN3_INCLUDE_DIR}/unsupported)
