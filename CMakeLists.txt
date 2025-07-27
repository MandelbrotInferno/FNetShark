cmake_minimum_required(VERSION 3.25)

project(FNetShark
    VERSION 1.0
    DESCRIPTION "Cross-platform C++20 file server"
    LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Platform-specific defines
if(WIN32)
    message(STATUS "Target platform: Windows")
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX)
    message(STATUS "Target platform: Linux/Unix")
    add_definitions(-DPLATFORM_LINUX)
endif()

# Include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# Recursively find all source and header files
file(GLOB_RECURSE FNETSHARK_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

# Define executable
add_executable(FNetShark ${FNETSHARK_SOURCES})

# Compiler warnings
if(MSVC)
    target_compile_options(FNetShark PRIVATE /W4 /permissive-)
else()
    target_compile_options(FNetShark PRIVATE -Wall -Wextra -pedantic)
endif()
