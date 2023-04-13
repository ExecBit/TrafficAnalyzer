#pragma once
#include <iostream>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/RawPacket.h>

class HttpPacketStat 
{
    public:
    HttpPacketStat();
    void print_stat();
    void consume_packet(pcpp::Packet http_packet);

    private:
    int m_http_packet_count;
};