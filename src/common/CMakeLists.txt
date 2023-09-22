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

# FIXME: should be added via a spdlog::spdlog target
find_package(Threads REQUIRED)
target_link_libraries(objcommon PUBLIC Threads::Threads)

file(GLOB_RECURSE MY_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
set_target_properties(objcommon PROPERTIES PUBLIC_HEADER "${MY_PUBLIC_HEADERS}")
install(TARGETS objcommon PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/novatel/edie)
