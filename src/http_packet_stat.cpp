#include "http_packet_stat.hpp"

HttpPacketStat::HttpPacketStat() : m_http_packet_count(0)
{

}

void HttpPacketStat::print_final_stat()
{
    std::cout << "Print final statictic\n" <<
        m_http_packet_count << '\n';
}
void HttpPacketStat::print_current_stat()
{
    std::cout << "Count of http packet:\t" << m_http_packet_count << '\n';

}
void HttpPacketStat::consume_packet(pcpp::Packet http_packet)
{
    ++m_http_packet_count;
}