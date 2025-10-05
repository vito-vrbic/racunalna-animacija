cmake_minimum_required(VERSION 3.15)
project(LAB2)

set(CMAKE_CXX_STANDARD 17)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/output)

# Dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# GLM (header-only)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# STD_IMAGE (header-only)
set(STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/stb_image)

# Source files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/sources/*.cpp")

# Add GLAD source from dependencies folder
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/dependencies/glad/src/glad.c")

# Create executable
add_executable(LAB2 ${SRC_FILES})

# Link libraries
target_link_libraries(LAB2 PRIVATE glfw)

# Include directories
target_include_directories(LAB2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers                 # Local headers
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include   # GLAD headers
    ${glm_SOURCE_DIR}                                       # GLM headers
    ${STB_INCLUDE_DIR}                                      # stb_image headers
)

# Copy shaders folder to the output directory of each build configuration
add_custom_command(TARGET LAB2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:LAB2>/shaders
)

# Copy assets folder to the output directory of each build configuration
add_custom_command(TARGET LAB2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:LAB2>/assets
)
