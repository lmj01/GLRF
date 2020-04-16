include(GoogleTest)

macro(google_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${PROJECT_NAME})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()

google_add_test(${PROJECT_NAME}_test_PlaneGenerator "PlaneGeneratorTest.cpp")