cmake_minimum_required(VERSION 3.15)
project(LAB1)

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

# Source files
file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/sources/*.cpp")

# Add GLAD source from dependencies folder
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/dependencies/glad/src/glad.c")

# Create executable
add_executable(LAB1 ${SRC_FILES})

# Link libraries
target_link_libraries(LAB1 PRIVATE glfw)

# Include directories
target_include_directories(LAB1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers                 # Local headers
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include   # GLAD headers
    ${glm_SOURCE_DIR}                                       # GLM headers
)

# Copy shaders folder to the output directory of each build configuration
add_custom_command(TARGET LAB1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders
    $<TARGET_FILE_DIR:LAB1>/shaders
)