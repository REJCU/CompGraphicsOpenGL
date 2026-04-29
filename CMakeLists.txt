cmake_minimum_required(VERSION 3.10)
project(ShaderStudy C)

# 1. FIX OPENGL WARNINGS (CMP0072)
# This tells CMake to use the modern GLVND system on Linux
set(OpenGL_GL_PREFERENCE GLVND)

# 2. CONFIGURE C SETTINGS
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 3. CONFIGURE GLFW (Embedded Source)
# We disable parts of GLFW we don't need to speed up compilation
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Point this to the folder where your GLFW files (README, src, etc.) live
# Assuming your structure is: ProjectRoot/external/glfw
add_subdirectory(external/glfw)

# 4. DEFINE THE EXECUTABLE
# We include glad.c here because it needs to be compiled into the binary
add_executable(${PROJECT_NAME} 
    src/main.c 
    src/glad.c
)

# 5. INCLUDE DIRECTORIES
# Tells the compiler where to look for glad.h and glfw3.h
target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
    external/glfw/include
)

# 6. LINK LIBRARIES
# We find the system's OpenGL drivers and link them + GLFW to our project
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw 
    ${OPENGL_LIBRARIES}
)

# Create a link to the shaders folder in the build directory
add_custom_command(TARGET ShaderStudy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders)
