#pragma once
#include <iostream>
#include <map>

#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>
#include <pcapplusplus/HttpLayer.h>
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/ProtocolType.h>
#include <pcapplusplus/TextBasedProtocol.h>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

namespace pstat 
{

    struct HostHttpStat 
    {
        HostHttpStat();
        int count_request_packet_http;
        int count_response_packet_http;

        size_t input_payload;
        size_t output_payload;
    };


    class HttpPacketStat 
    {
        public:

        HttpPacketStat();
        void print_current_stat();
        void print_final_stat();
        void consume_packet(pcpp::Packet* http_packet);

        private:

        std::map<std::string, HostHttpStat> host_table;

        log4cplus::Logger logger;
        log4cplus::BasicConfigurator config;
    };
}