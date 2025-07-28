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

# Only add headers from include/ to the compiler's include path
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files from src/
file(GLOB_RECURSE FNETSHARK_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

# Collect only header files from include/
file(GLOB_RECURSE FNETSHARK_HEADERS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/include/*.h"
)

# Combine for filter grouping and IDE visibility
set(FNETSHARK_ALL_FILES ${FNETSHARK_SOURCES} ${FNETSHARK_HEADERS})

# --- Visual Studio filters (source groups) ---
function(group_sources_by_folder FILES_LIST)
    foreach(file_path IN LISTS FILES_LIST)
        file(RELATIVE_PATH rel_path "${PROJECT_SOURCE_DIR}" "${file_path}")
        get_filename_component(dir "${rel_path}" DIRECTORY)
        string(REPLACE "/" "\\" group "${dir}")
        source_group("${group}" FILES "${file_path}")
    endforeach()
endfunction()

group_sources_by_folder("${FNETSHARK_ALL_FILES}")

# Define executable with source + header files
add_executable(FNetShark ${FNETSHARK_SOURCES} ${FNETSHARK_HEADERS})

# Link fmt from vcpkg
find_package(fmt CONFIG REQUIRED)
target_link_libraries(FNetShark PRIVATE fmt::fmt)

# Compiler warnings
if(MSVC)
    target_compile_options(FNetShark PRIVATE /W4 /permissive-)
else()
    target_compile_options(FNetShark PRIVATE -Wall -Wextra -pedantic)
endif()
