cmake_minimum_required(VERSION 3.15)
include(cmake/third_party.cmake)

project(EDIE VERSION 3.2.24 LANGUAGES CXX)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DYNAMIC_LIBS "Build additional C-style *_dynamic_library versions of the libraries" ON)
option(COVERAGE "Coverage" OFF)

set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()

if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(NOT DEFINED CMAKE_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE OFF)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
message("CMAKE_GENERATOR_PLATFORM: " ${CMAKE_GENERATOR_PLATFORM})
message("CMAKE_GENERATOR_TOOLSET: " ${CMAKE_GENERATOR_TOOLSET})

if(MSVC)
    if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        if(BUILD_SHARED_LIBS)
            # shared spdlog requires MultiThreadedDLL
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        endif()
    endif()
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/utf-8 /W4 $<$<CONFIG:Debug>:/Zi>)
else()
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
        add_compile_options(-m32)
    endif()
    add_compile_options(-Wall)
    add_compile_options("$<$<CONFIG:Debug>:-g>")
    add_compile_options("$<$<CONFIG:Release>:-O3>")
    add_link_options("$<$<CONFIG:Release>:-s>")
    if(COVERAGE)
        message("Coverage is On")
        add_compile_options($<$<CONFIG:Release>:--coverage>)
    endif()
endif()

# Configure installation directories
set(CMAKE_INSTALL_INCLUDEDIR include CACHE PATH "")
set(CMAKE_INSTALL_BINDIR bin/novatel/edie CACHE PATH "")
set(CMAKE_INSTALL_LIBDIR lib/novatel/edie CACHE PATH "")
set(CMAKE_INSTALL_DATADIR share/novatel/edie CACHE PATH "")

# Output all binaries in the same directory for easier testing
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# build EDIE components
add_subdirectory(src)

# Find Git package, if not need to install manually or through .yml file
find_package(Git)
if(Git_FOUND)
    message("Git found: ${GIT_EXECUTABLE}")
    execute_process(COMMAND ${CMAKE_COMMAND}
        -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
        -D DST=${CMAKE_CURRENT_SOURCE_DIR}/src/common/include/novatel/edie/version.h
        -D GIT_EXECUTABLE=${GIT_EXECUTABLE}
        -D GIT_BRANCH=${GIT_BRANCH}
        -D EDIE_VERSION=${EDIE_VERSION}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateVersionHeader.cmake)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(src/common/test)
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

add_library(EDIE INTERFACE)
target_link_libraries(EDIE INTERFACE EDIE::novatel EDIE::stream_interface)
install(TARGETS EDIE EXPORT EDIE-targets)

if(BUILD_DYNAMIC_LIBS)
    add_library(EDIE-dynamic INTERFACE)
    target_link_libraries(EDIE-dynamic INTERFACE EDIE::decoders_dynamic_library EDIE::hwinterface_dynamic_library)
    install(TARGETS EDIE-dynamic EXPORT EDIE-targets)
endif()

set(EDIE_CMAKE_INSTALL_DIR lib/cmake/EDIE)
install(EXPORT EDIE-targets
    NAMESPACE EDIE::
    DESTINATION "${EDIE_CMAKE_INSTALL_DIR}"
    FILE edie-targets.cmake
    COMPONENT EDIE)

# Install exported targets and cmake files
include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/edie-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/edie-config.cmake"
    INSTALL_DESTINATION ${EDIE_CMAKE_INSTALL_DIR}
    PATH_VARS
        CMAKE_INSTALL_INCLUDEDIR
        CMAKE_INSTALL_LIBDIR
        CMAKE_INSTALL_BINDIR
        CMAKE_INSTALL_DATADIR)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/edie-config-version.cmake"
    VERSION ${EDIE_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/edie-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/edie-config-version.cmake"
    DESTINATION "${EDIE_CMAKE_INSTALL_DIR}"
    COMPONENT EDIE)

if(MSVC)
    copy_third_party_dlls()
endif()
if(INSTALL_THIRD_PARTY)
    install_third_party_libs()
endif()

# Add uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
