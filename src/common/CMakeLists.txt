file(GLOB_RECURSE COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

# Creating an object file which can be used for both static and shared build without recompilation.
add_library(objcommon OBJECT ${COMMON_SOURCES})

set_property(TARGET objcommon PROPERTY FOLDER "common")

target_include_directories(objcommon
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/common>"
)

if(BUILD_SHARED_LIBS)
    # Statically linking against spdlog causes its singleton registry to be
    # re-instantiated in each shared library and executable that links against it.
    list(APPEND CONAN_EXTRA_INSTALL_ARGS
        -o spdlog/*:shared=True -o fmt/*:shared=True
    )
endif()

find_package(nlohmann_json REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(spdlog_setup REQUIRED CONFIG)

add_library(common_dependencies INTERFACE)
add_library(EDIE::common_dependencies ALIAS common_dependencies)
target_link_libraries(common_dependencies INTERFACE
    nlohmann_json::nlohmann_json
    spdlog_setup::spdlog_setup
)
if(TARGET spdlog::spdlog)
    target_link_libraries(common_dependencies INTERFACE spdlog::spdlog)
else()
    target_link_libraries(common_dependencies INTERFACE spdlog::spdlog_header_only)
endif()

target_link_libraries(objcommon PUBLIC EDIE::common_dependencies)

install(TARGETS common_dependencies EXPORT EDIE-targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
