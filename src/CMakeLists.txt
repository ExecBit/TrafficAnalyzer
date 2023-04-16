cmake_minimum_required(VERSION 3.11)

project(Traffic_Analyzer)

include(FetchContent)

FetchContent_Declare(
    PcapPlusPlus
    GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
    GIT_TAG master
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/pcapplusplus
)

FetchContent_MakeAvailable(PcapPlusPlus)

add_executable(${PROJECT_NAME} 
    main.cpp
    http_packet_stat.cpp
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    Common++
    Packet++
    Pcap++
    log4cplus
)



