#include <gtest/gtest.h>
#include "../src/http_packet_stat.hpp"

TEST(DEFAULT, CountHttpTypePacket)
{
    pstat::HttpPacketStat stat;


    pcpp::PcapFileReaderDevice reader("test_http_dump.pcap");
    if (!reader.open())
    {
        std::cerr << "Error opening the pcap file" << std::endl;
    }

    pcpp::RawPacket raw_packet;

    while (reader.getNextPacket(raw_packet))
    {
        pcpp::Packet parsed_packet(&raw_packet);
        stat.consume_packet(&parsed_packet);
    }

    reader.close();

    EXPECT_EQ(596, stat.get_count_http_packet());

}

TEST(DEFAULT, CountHttpRequestPacket)
{
    pstat::HttpPacketStat stat;


    pcpp::PcapFileReaderDevice reader("test_http_dump.pcap");
    if (!reader.open())
    {
        std::cerr << "Error opening the pcap file" << std::endl;
    }

    pcpp::RawPacket raw_packet;

    while (reader.getNextPacket(raw_packet))
    {
        pcpp::Packet parsed_packet(&raw_packet);
        stat.consume_packet(&parsed_packet);
    }

    reader.close();

    EXPECT_EQ(298, stat.get_count_request_http_packet());

}

TEST(DEFAULT, CountHttpResponsePacket)
{
    pstat::HttpPacketStat stat;


    pcpp::PcapFileReaderDevice reader("test_http_dump.pcap");
    if (!reader.open())
    {
        std::cerr << "Error opening the pcap file" << std::endl;
    }

    pcpp::RawPacket raw_packet;

    while (reader.getNextPacket(raw_packet))
    {
        pcpp::Packet parsed_packet(&raw_packet);
        stat.consume_packet(&parsed_packet);
    }

    reader.close();

    EXPECT_EQ(298, stat.get_count_response_http_packet());

}