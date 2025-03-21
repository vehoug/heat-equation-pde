cmake_minimum_required(VERSION 3.10)
project(heat_equation_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    if(EXISTS "${CMAKE_SOURCE_DIR}/external/glfw/CMakeLists.txt")
        option(GLFW_BUILD_DOCS OFF)
        option(GLFW_BUILD_EXAMPLES OFF)
        option(GLFW_BUILD_TESTS OFF)
        add_subdirectory(external/glfw)
    else()
        message(FATAL_ERROR "GLFW not found. Please install GLFW or clone it into external/glfw")
    endif()
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
    add_library(glad external/glad/src/glad.c)
    target_include_directories(glad PUBLIC external/glad/include)
else()
    message(FATAL_ERROR "GLAD source not found. Please place GLAD in external/glad")
endif()

find_package(glm QUIET)
if(NOT glm_FOUND)
    if(EXISTS "${CMAKE_SOURCE_DIR}/external/glm/CMakeLists.txt")
        add_subdirectory(external/glm)
    else()
        if(EXISTS "${CMAKE_SOURCE_DIR}/external/glm/glm/glm.hpp")
        else()
            message(FATAL_ERROR "GLM not found. Please install GLM or clone it into external/glm")
        endif()
    endif()
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

if(glm_FOUND)
elseif(EXISTS "${CMAKE_SOURCE_DIR}/external/glm/CMakeLists.txt")
else()
    include_directories(${CMAKE_SOURCE_DIR}/external/glm)
endif()

if(NOT glfw3_FOUND)
    include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
endif()

set(SOURCES
    src/main.cpp
    src/renderer.cpp
    src/heat_solver.cpp
    src/shader.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glad
)

if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} glfw)
else()
    target_link_libraries(${PROJECT_NAME} glfw)
endif()

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})