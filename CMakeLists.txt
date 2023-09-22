include(cmake/conan.cmake)

cmake_minimum_required(VERSION 3.15)
project(EDIE LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DYNAMIC_LIBS "Build additional C-style *_dynamic_library versions of the libraries" ON)
option(COVERAGE "Coverage" OFF)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(NOT DEFINED CMAKE_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE OFF)
endif()

# Check platforms
if(CMAKE_HOST_WIN32)
    set(WINDOWS 1)
    message("CMAKE Host: Windows")
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
    message("CMAKE Host: Linux")
endif()

message("CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
message("BUILD_SHARED_LIBS: " ${BUILD_SHARED_LIBS})

if(WINDOWS)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/MT /utf-8 $<$<CONFIG:Debug>:/Zi>)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/W4>)
    message("CMAKE_GENERATOR_PLATFORM: " ${CMAKE_GENERATOR_PLATFORM})
    message("CMAKE_GENERATOR_TOOLSET: " ${CMAKE_GENERATOR_TOOLSET})
elseif(LINUX)
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
        add_compile_options(-m32)
    endif()

    add_compile_options($<$<CONFIG:Debug>:-g>)
    add_compile_options("$<$<CONFIG:Release>:-Wall;-O3>")
    add_link_options($<$<CONFIG:Release>:-s>)

    if(COVERAGE)
        message("Coverage is On")
        add_compile_options($<$<CONFIG:Release>:--coverage>)
    endif()
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# build EDIE components
add_subdirectory(src)

# Find Git package, if not need to install manually or through .yml file
find_package(Git)
if(Git_FOUND)
    message("Git found: ${GIT_EXECUTABLE}")
    execute_process(COMMAND ${CMAKE_COMMAND}
        -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
        -D DST=${CMAKE_CURRENT_SOURCE_DIR}/src/common/include/version.h
        -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
        -D GIT_BRANCH=${GIT_BRANCH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake)
endif()

if(BUILD_TESTS)
    add_subdirectory(src/common/test)
    add_subdirectory(src/decoders/novatel/test)
    add_subdirectory(src/hw_interface/stream_interface/test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
