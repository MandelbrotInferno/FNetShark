cmake_minimum_required(VERSION 3.25)

# Use local vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

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

# Recursively find all relevant source and header files
file(GLOB_RECURSE FNETSHARK_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

# --- Visual Studio filters (source groups) ---
function(group_sources_by_folder FILES_LIST)
    foreach(file_path IN LISTS FILES_LIST)
        file(RELATIVE_PATH rel_path "${PROJECT_SOURCE_DIR}" "${file_path}")
        get_filename_component(dir "${rel_path}" DIRECTORY)
        string(REPLACE "/" "\\" group "${dir}")
        source_group("${group}" FILES "${file_path}")
    endforeach()
endfunction()

group_sources_by_folder("${FNETSHARK_SOURCES}")

# Define executable
add_executable(FNetShark ${FNETSHARK_SOURCES})

# Link fmt from vcpkg
find_package(fmt CONFIG REQUIRED)
target_link_libraries(FNetShark PRIVATE fmt::fmt)

# Compiler warnings
if(MSVC)
    target_compile_options(FNetShark PRIVATE /W4 /permissive-)
else()
    target_compile_options(FNetShark PRIVATE -Wall -Wextra -pedantic)
endif()
