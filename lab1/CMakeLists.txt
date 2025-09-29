cmake_minimum_required(VERSION 3.15)
project(Lab1_OpenGL_Assimp)

set(CMAKE_CXX_STANDARD 17)

# Dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glad)

# Assimp
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG master
)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# Build lab1 executable
add_executable(lab1
    src/main.cpp
)

# Link dependencies
target_link_libraries(lab1
    glfw
    glad
    assimp
)

# Add include folder
target_include_directories(lab1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include
    ${glad_SOURCE_DIR}/include
    ${assimp_SOURCE_DIR}/include
)
