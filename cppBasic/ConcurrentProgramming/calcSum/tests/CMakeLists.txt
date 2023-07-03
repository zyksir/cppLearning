include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.0-preview3)

FetchContent_MakeAvailable(catch2)

macro(calcsum_add_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    add_test(NAME calcsum.${TEST_NAME} COMMAND ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE libcalcsum)
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain)
endmacro()

calcsum_add_test(test_concurrent_calcSum test_concurrent_calcSum.cpp)
