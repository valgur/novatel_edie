file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
)
set(HWINTERFACE_DYNAMIC_LIBRARY_SOURCES)
list(APPEND HWINTERFACE_DYNAMIC_LIBRARY_SOURCES ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HWINTERFACE_DYNAMIC_LIBRARY_SOURCES})

#Creating a shared/dynamic lib, with the project name, using available object files, without recompilation.
add_library(hwinterface_dynamic_library SHARED ${HWINTERFACE_DYNAMIC_LIBRARY_SOURCES})
add_library(edie::hwinterface_dynamic_library ALIAS hwinterface_dynamic_library)
target_link_libraries(hwinterface_dynamic_library PUBLIC obj::stream_interface obj::common)

set_property(TARGET hwinterface_dynamic_library PROPERTY FOLDER "hw_interface")

if (WINDOWS)
    add_compile_definitions(HWINTERFACE_EXPORTS)
else()
    add_compile_definitions(HWINTERFACE_STATIC_DEFINE)
endif()

target_include_directories(hwinterface_dynamic_library
    PUBLIC include
    PRIVATE include/edie/hw_interface/dynamic_library
)

install(TARGETS hwinterface_dynamic_library COMPONENT dynamic_libs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
