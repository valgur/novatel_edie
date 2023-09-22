file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(DECODERS_DYNAMIC_LIBRARY_SOURCES)
list(APPEND DECODERS_DYNAMIC_LIBRARY_SOURCES ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DECODERS_DYNAMIC_LIBRARY_SOURCES})

#Creating a shared/dynamic lib, with the project name, using available object files, without recompilation.
add_library(decoders_dynamic_library SHARED ${DECODERS_DYNAMIC_LIBRARY_SOURCES})
add_library(edie::decoders_dynamic_library ALIAS decoders_dynamic_library)
target_link_libraries(decoders_dynamic_library PUBLIC obj::common obj::novatel)
target_link_libraries(decoders_dynamic_library PUBLIC edie::hwinterface_dynamic_library)

set_property(TARGET decoders_dynamic_library PROPERTY FOLDER "decoders")

if (WINDOWS)
    add_compile_definitions(DECODERS_EXPORTS)
else()
    add_compile_definitions(DECODERS_STATIC_DEFINE)
endif()

target_include_directories(decoders_dynamic_library
    PUBLIC include
    PRIVATE include/edie/decoders/dynamic_library
)

install(TARGETS decoders_dynamic_library COMPONENT dynamic_libs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
