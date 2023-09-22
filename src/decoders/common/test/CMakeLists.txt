file (GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)
set(COMMONTEST_SOURCES)
list(APPEND COMMONTEST_SOURCES ${SOURCES})

find_package(GTest REQUIRED CONFIG)

add_executable(CommonTest ${COMMONTEST_SOURCES})
set_property(TARGET CommonTest PROPERTY FOLDER "edie/decoders/tests")
target_link_libraries(CommonTest PUBLIC
    edie::novatel
    GTest::GTest GTest::Main
)

add_test(CommonTest COMMAND CommonTest)
