file(GLOB NOVATELTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
)

find_package(GTest REQUIRED CONFIG)

add_executable(NovatelTest ${NOVATELTEST_SOURCES})
set_property(TARGET NovatelTest PROPERTY FOLDER "decoders/tests")
target_link_libraries(NovatelTest PUBLIC
    EDIE::novatel EDIE::stream_interface
    GTest::gtest GTest::gtest_main
)

add_test(
    NAME NovatelTest
    COMMAND NovatelTest ${CMAKE_SOURCE_DIR}/database/messages_public.json ${CMAKE_CURRENT_LIST_DIR}/resources
)
