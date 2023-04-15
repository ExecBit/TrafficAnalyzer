#pragma once
#include <iostream>
#include <map>
#include <string>
#include <cstddef>
#include <exception>
#include <getopt.h>

#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>
#include <pcapplusplus/HttpLayer.h>
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/ProtocolType.h>
#include <pcapplusplus/TextBasedProtocol.h>
#include <pcapplusplus/IPLayer.h>
#include <pcapplusplus/TablePrinter.h>
#include <pcapplusplus/SystemUtils.h>
#include <pcapplusplus/Device.h>
#include <pcapplusplus/PcapFilter.h>
#include <pcapplusplus/ProtocolType.h>
#include <pcapplusplus/PcapLiveDevice.h>
#include <pcapplusplus/PcapLiveDeviceList.h>

#include <log4cplus/initializer.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/tchar.h>

namespace pstat 
{

    struct HostHttpStat 
    {
        int count_request_packet_http;
        int count_response_packet_http;

        size_t input_payload;
        size_t output_payload;

        std::string host_name;
    };


    class HttpPacketStat 
    {
        public:

        HttpPacketStat();
        void print_stat();
        void consume_packet(pcpp::Packet* http_packet);

        private:

        std::map<std::string, HostHttpStat> m_host_table;

        log4cplus::Logger m_logger;
        log4cplus::BasicConfigurator config;
    };
}