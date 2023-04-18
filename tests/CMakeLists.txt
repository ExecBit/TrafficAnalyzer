cmake_minimum_required(VERSION 3.11)

project(Traffic_Analyzer_test)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main 
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdParty/googletest
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} http_packet_stat_test.cpp ../src/http_packet_stat.cpp)

add_executable(reader reader.cpp ../src/http_packet_stat.cpp)

target_link_libraries(reader PUBLIC
    Common++
    Packet++
    Pcap++
    log4cplus
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    gtest_main
    gmock_main
    Common++
    Packet++
    Pcap++
    log4cplus
)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})
