#pragma once
#include <iostream>
#include <map>
#include <string>
#include <cstddef>
#include <exception>
#include <getopt.h>

#include <log4cplus/initializer.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>
#include <log4cplus/tchar.h>

#include <Packet.h>
#include <RawPacket.h>
#include <HttpLayer.h>
#include <TcpLayer.h>
#include <ProtocolType.h>
#include <TextBasedProtocol.h>
#include <IPLayer.h>
#include <TablePrinter.h>
#include <SystemUtils.h>
#include <Device.h>
#include <PcapFilter.h>
#include <ProtocolType.h>
#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>


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