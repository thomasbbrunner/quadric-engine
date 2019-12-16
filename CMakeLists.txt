
# Project
cmake_minimum_required(VERSION 2.8)
project(quadricworks)

# CMake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

# Executables
message(STATUS "Looking for main file in ${PROJECT_SOURCE_DIR}")
add_executable(quadricworks 
    ${PROJECT_SOURCE_DIR}/study.cpp 
    ${PROJECT_SOURCE_DIR}/inc/buffer.cpp
    ${PROJECT_SOURCE_DIR}/inc/camera.cpp
    ${PROJECT_SOURCE_DIR}/inc/math.cpp
    ${PROJECT_SOURCE_DIR}/inc/print.cpp
    ${PROJECT_SOURCE_DIR}/inc/thing.cpp
    ${PROJECT_SOURCE_DIR}/lib/glad/glad.c)

# Headers
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/lib/glad)

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND) # forcing use of GLVND libs instead of legacy
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL in ${OPENGL_gl_LIBRARY}")
target_link_libraries(quadricworks ${OPENGL_gl_LIBRARY})

# GLM
find_package(glm REQUIRED)
message(STATUS "Found glm in ${glm_DIR}")
target_link_libraries(quadricworks ${GLM_INCLUDE_DIRS})

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found glfw3 in ${glfw3_DIR}")
target_link_libraries(quadricworks glfw) 

# Glad
set(GLAD_DIR ${PROJECT_SOURCE_DIR}/lib/glad)
add_library(glad ${GLAD_DIR}/glad.c)
target_link_libraries(glad ${CMAKE_DL_LIBS})
target_include_directories(glad PUBLIC ${GLAD_DIR})
target_include_directories(quadricworks PUBLIC ${GLAD_DIR})
target_link_libraries(quadricworks glad)# "${CMAKE_DL_LIBS}")

# add_library(glad ${PROJECT_SOURCE_DIR}/lib/glad/glad.h ${PROJECT_SOURCE_DIR}/lib/glad/glad.c)
#target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/lib/glad/)
# target_link_libraries(quadricworks glad)

# Eigen
find_package(Eigen3 REQUIRED)
message(STATUS "Found Eigen in ${Eigen3_DIR}")
target_link_libraries(quadricworks Eigen3::Eigen)
include_directories(${EIGEN3_INCLUDE_DIR}/unsupported)
