include(cmake/conan.cmake)

cmake_minimum_required(VERSION 3.15)
project(edie-public VERSION 3.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DYNAMIC_LIBS "Build additional C-style *_dynamic_library versions of the libraries" ON)
option(COVERAGE "Coverage" OFF)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(STRICT)

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

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    # FIXME: add missing exports to decoders
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    add_compile_options(/utf-8 $<$<CONFIG:Debug>:/Zi>)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/W4>)
    message("CMAKE_GENERATOR_PLATFORM: " ${CMAKE_GENERATOR_PLATFORM})
    message("CMAKE_GENERATOR_TOOLSET: " ${CMAKE_GENERATOR_TOOLSET})
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -fPIC -Wfatal-errors -Wold-style-cast -Wextra -pedantic)
    add_compile_options("$<$<CONFIG:Release>:-O3>")

    if(LINUX)
        add_definitions(-D_LINUX_)
    endif()
    if(WINDOWS)
        add_compile_options($<$<CONFIG:Debug>:-O1>)
    endif()
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Werror -Wno-format-truncation -Wno-error=format -Wno-format -Wno-format-extra-args)
    endif()
    if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-pthread -Wno-unqualified-std-cast-call)
    endif()

    if(COVERAGE)
        message("Coverage is On")
        add_compile_options(--coverage)
    endif()
endif()

# Configure installation directories
set(CMAKE_INSTALL_INCLUDEDIR include CACHE PATH "")
set(CMAKE_INSTALL_BINDIR bin/novatel/edie CACHE PATH "")
set(CMAKE_INSTALL_LIBDIR lib/novatel/edie CACHE PATH "")
set(CMAKE_INSTALL_DATADIR share/novatel/edie CACHE PATH "")

# build EDIE components
add_subdirectory(src)

# Find Git package, if not need to install manually or through .yml file
find_package(Git)
if(Git_FOUND)
    message("Git found: ${GIT_EXECUTABLE}")
    execute_process(COMMAND ${CMAKE_COMMAND}
        -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
        -D DST=${CMAKE_CURRENT_SOURCE_DIR}/src/decoders/common/include/edie/version.h
        -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
        -D GIT_BRANCH=${GIT_BRANCH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(src/decoders/common/test)
    add_subdirectory(src/decoders/novatel/test)
    add_subdirectory(src/hw_interface/stream_interface/test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(FILES database/messages_public.json
    DESTINATION ${CMAKE_INSTALL_DATADIR}
    COMPONENT data
)
