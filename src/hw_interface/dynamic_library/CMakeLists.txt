set(TARGET_NAME "hw_interface_dynamic_library")
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${TARGET_NAME} SHARED ${SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE common stream_interface)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "hw_interface")

if(WIN32)
    add_compile_definitions(HWINTERFACE_EXPORTS)
else()
    add_compile_definitions(HWINTERFACE_STATIC_DEFINE)
endif()

target_include_directories(${TARGET_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE include/edie/hw_interface/dynamic_library
)

install(TARGETS ${TARGET_NAME}
    EXPORT EDIE-targets
    COMPONENT dynamic_libs
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
