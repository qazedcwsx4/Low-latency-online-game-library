enable_testing()

ADD_EXECUTABLE(testInit testInit.cpp)
ADD_EXECUTABLE(testSend testSend.cpp)

target_link_libraries(testInit lil)
target_link_libraries(testSend lil)
IF (WIN32)
    target_link_libraries(testInit WS2_32.lib)
    target_link_libraries(testSend WS2_32.lib)
ENDIF ()
ADD_TEST(testInit testInit)
ADD_TEST(testSend testSend)


add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS testInit testSend)
