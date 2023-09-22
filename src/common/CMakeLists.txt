file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(COMMON_SOURCES)
list(APPEND COMMON_SOURCES ${SOURCES})

# Creating an object file which can be used for both static and shared build without recompilation.
add_library(objcommon OBJECT ${COMMON_SOURCES})
add_library(obj::common ALIAS objcommon)

set_property(TARGET objcommon PROPERTY FOLDER "decoders")

target_include_directories(objcommon
    PUBLIC include
    PUBLIC include/common
)

find_package(nlohmann_json REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
target_link_libraries(objcommon PUBLIC nlohmann_json::nlohmann_json spdlog::spdlog)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
