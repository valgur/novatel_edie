file(GLOB STREAMINTERFACETEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

find_package(GTest REQUIRED CONFIG)

add_executable(StreamInterfaceTest ${STREAMINTERFACETEST_SOURCES} $<TARGET_OBJECTS:objcommon>)
set_property(TARGET StreamInterfaceTest PROPERTY FOLDER "hw_interface/tests")
target_link_libraries(StreamInterfaceTest PUBLIC
    EDIE::stream_interface
    GTest::gtest GTest::gtest_main
)

add_test(
    NAME StreamInterfaceTest
    COMMAND StreamInterfaceTest ${CMAKE_CURRENT_LIST_DIR}/resources
)
