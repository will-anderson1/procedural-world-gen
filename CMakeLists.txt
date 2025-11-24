cmake_minimum_required(VERSION 3.14)
project(ProceduralWorldGen)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(glm REQUIRED)

set(SDL3_FRAMEWORK_PATH $ENV{HOME}/Library/Frameworks)
find_library(SDL3_LIBRARY NAMES SDL3 PATHS ${SDL3_FRAMEWORK_PATH} REQUIRED)
include_directories(${SDL3_FRAMEWORK_PATH}/SDL3.framework/Headers)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})


add_executable(ProceduralWorldGen main.cpp impl/HeightMap.cpp impl/Noise.cpp impl/utils.cpp impl/InputHandler.cpp headers/glad/glad.c)

# Link Libraries
target_link_libraries(ProceduralWorldGen PRIVATE glm::glm ${SDL3_LIBRARY} ${OPENGL_LIBRARIES})

# Configure RPATH for macOS Frameworks
if(APPLE)
    set_target_properties(ProceduralWorldGen PROPERTIES BUILD_RPATH ${SDL3_FRAMEWORK_PATH})
    set_target_properties(ProceduralWorldGen PROPERTIES INSTALL_RPATH ${SDL3_FRAMEWORK_PATH})
endif()
