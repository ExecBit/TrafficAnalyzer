cmake_minimum_required(VERSION 3.11)

project(Traffic_Analyzer_test)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main 
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/googletest
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} main_test.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC
    gtest_main
    gmock_main
)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
