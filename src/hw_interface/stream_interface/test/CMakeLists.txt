file (GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(STREAMINTERFACETEST_SOURCES)
list(APPEND STREAMINTERFACETEST_SOURCES ${SOURCES})

find_package(GTest REQUIRED)

add_executable(StreamInterfaceTest ${STREAMINTERFACETEST_SOURCES})
set_property(TARGET StreamInterfaceTest PROPERTY FOLDER "hw_interface/tests")
target_link_libraries(StreamInterfaceTest PUBLIC
    edie::stream_interface
    GTest::GTest GTest::Main
)

add_test(StreamInterfaceTest COMMAND StreamInterfaceTest)
