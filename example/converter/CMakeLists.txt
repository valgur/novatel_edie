cmake_minimum_required(VERSION 3.12.4)
project(ConverterExample VERSION 1.0.0)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
set(CONVERTER_SOURCES)
LIST(APPEND CONVERTER_SOURCES ${SOURCES})

if(WINDOWS)
    source_group("src" FILES ${CONVERTER_SOURCES})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}-${ARCH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}-${ARCH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${PROJECT_NAME}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}-${ARCH})

set(PROJECT_LINK_LIBS hwinterface_dynamic_library decoders_dynamic_library)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../bin/hwinterface_dynamic_library/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}-${ARCH})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../bin/decoders_dynamic_library/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}-${ARCH})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_executable(converter main.cpp)
target_link_libraries(converter ${PROJECT_LINK_LIBS})
