#include "http_packet_stat.hpp"

namespace pstat 
{
    HttpPacketStat::HttpPacketStat() : m_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("http_packet_stat_class")))
    {
    }

    void HttpPacketStat::print_stat()
    {
        if (m_host_table.empty())
        {
            LOG4CPLUS_INFO(m_logger, LOG4CPLUS_TEXT("NO PACKAGES FOUND"));
            return;
        }

        std::vector<std::string> column_names = {"Host name", "IP address", "Number of Packets (IN/OUT)", "Traffic (IN/OUT)"};
        std::vector<int> column_sizes = {30,20,30, 30};

        pcpp::TablePrinter table(column_names, column_sizes);

        for (auto i = m_host_table.begin(); i != m_host_table.end(); ++i)
        {

            std::vector<std::string> val{
                i->second.host_name,
                i->first,
                std::to_string(i->second.count_response_packet_http + i->second.count_request_packet_http) +
                " (" + std::to_string(i->second.count_response_packet_http) + " / " + std::to_string(i->second.count_request_packet_http) + ')',
                std::to_string(i->second.input_payload + i->second.output_payload) + 
                " B (" + std::to_string(i->second.input_payload) + "B / " + std::to_string(i->second.output_payload) + "B)"};


            table.printRow(val);
        }
    }

    void HttpPacketStat::consume_packet(pcpp::Packet* http_packet)
    {
        //get tcp layer for counting payload
        pcpp::TcpLayer* tcp_layer = http_packet->getLayerOfType<pcpp::TcpLayer>();

        //get ip layer for host address
        pcpp::IPLayer* ip_layer = http_packet->getLayerOfType<pcpp::IPLayer>();

        if (http_packet->isPacketOfType(pcpp::HTTPRequest))
        {
            //get http request layer for counting packet and verify host
            pcpp::HttpRequestLayer* http_request = http_packet->getLayerOfType<pcpp::HttpRequestLayer>();
            pcpp::HeaderField* header_field = http_request->getFieldByName(PCPP_HTTP_HOST_FIELD);

            std::string host = ip_layer->getDstIPAddress().toString();

            if (m_host_table.find(host) == m_host_table.end())
            {
                m_host_table[host].host_name = header_field->getFieldValue();

            }

            //get data for statistic
            ++m_host_table[host].count_request_packet_http; 
            m_host_table[host].output_payload = tcp_layer->getLayerPayloadSize();
            
            return;
        }

        if (http_packet->isPacketOfType(pcpp::HTTPResponse))
        {
            //get http request layer for counting packet and verify host
            pcpp::HttpResponseLayer* http_response = http_packet->getLayerOfType<pcpp::HttpResponseLayer>();
            pcpp::HeaderField* header_field = http_response->getFieldByName(PCPP_HTTP_HOST_FIELD);

            std::string host = ip_layer->getSrcIPAddress().toString();

            if (m_host_table.find(host) == m_host_table.end())
            {
                m_host_table[host].host_name = header_field->getFieldValue();

            }

            //get data for statistic
            ++m_host_table[host].count_response_packet_http;
            m_host_table[host].input_payload = tcp_layer->getLayerPayloadSize();

            return;
        }

    }

}