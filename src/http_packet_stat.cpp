#include "http_packet_stat.hpp"
#include <cstddef>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/tchar.h>
#include <pcapplusplus/HttpLayer.h>
#include <pcapplusplus/IPLayer.h>
#include <pcapplusplus/ProtocolType.h>
#include <pcapplusplus/TcpLayer.h>
#include <pcapplusplus/TextBasedProtocol.h>

namespace pstat 
{
    HostHttpStat::HostHttpStat()
    {
        size_t count_request_packet_http{0};
        size_t count_response_packet_http{0};

        size_t input_payload{0};
        size_t output_payload{0};
    }

    HttpPacketStat::HttpPacketStat() 
    {
        config.configure();

        logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("http_packet_stat_class"));

    }


    void HttpPacketStat::print_final_stat()
    {
        //vk.com           100 packets (10 OUT / 90 IN)   Traffic: 150 KB (15KB OUT / 135KB IN)
        std::cout << "Print final statictic\n";

        for (auto i = host_table.begin(); i != host_table.end(); ++i)
        {
            std::cout << i->first << "\t" << i->second.count_request_packet_http + i->second.count_response_packet_http <<
            " packets (" << i->second.count_request_packet_http << " OUT / " << i->second.count_response_packet_http << " IN)\t" <<
            "Traffic: " << i->second.input_payload + i->second.output_payload << " KB (" << i->second.output_payload << "KB OUT / " << 
            i->second.input_payload << "KB IN)\n";
        }


    }
    void HttpPacketStat::print_current_stat()
    {
        std::cout << "Count of http packet:\n";

        for (auto i = host_table.begin(); i != host_table.end(); ++i)
        {
            std::cout << i->first << "\t" << i->second.count_request_packet_http + i->second.count_response_packet_http <<
            " packets (" << i->second.count_request_packet_http << " OUT / " << i->second.count_response_packet_http << " IN)\t" <<
            "Traffic: " << i->second.input_payload + i->second.output_payload << " KB (" << i->second.output_payload << "KB OUT / " << 
            i->second.input_payload << "KB IN)\n";
        }
    }
    void HttpPacketStat::consume_packet(pcpp::Packet* http_packet)
    {
      //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("handler http packet"));
        //get tcp layer for counting payload
        pcpp::TcpLayer* tcp_layer = http_packet->getLayerOfType<pcpp::TcpLayer>();

        pcpp::IPLayer* ip_layer = http_packet->getLayerOfType<pcpp::IPLayer>();

        if (http_packet->isPacketOfType(pcpp::HTTPRequest))
        {
            //get http request layer for counting packet and verify host
            pcpp::HttpRequestLayer* http_request = http_packet->getLayerOfType<pcpp::HttpRequestLayer>();
            pcpp::HeaderField* header_field = http_request->getFieldByName(PCPP_HTTP_HOST_FIELD);

            std::string host = ip_layer->getDstIPAddress().toString();

          //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("add stat to map (request)"));

            ++host_table[host].count_request_packet_http; 
            host_table[host].output_payload = tcp_layer->getLayerPayloadSize();

          //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("success adding stat to map (request)"));
            
            return;
        }

        if (http_packet->isPacketOfType(pcpp::HTTPResponse))
        {
            //get http request layer for counting packet and verify host
            pcpp::HttpResponseLayer* http_response = http_packet->getLayerOfType<pcpp::HttpResponseLayer>();
            pcpp::HeaderField* header_field = http_response->getFieldByName(PCPP_HTTP_HOST_FIELD);

            std::string host = ip_layer->getSrcIPAddress().toString();

          //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("add stat to map (response)"));

            ++host_table[host].count_response_packet_http;
            host_table[host].input_payload = tcp_layer->getLayerPayloadSize();

          //LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("success add stat to map (request)"));
            return;
        }

    }
}