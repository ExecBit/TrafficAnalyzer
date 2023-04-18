#include <gtest/gtest.h>
#include "../src/http_packet_stat.hpp"

#include <pcapplusplus/Device.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/PcapFileDevice.h>
#include <pcapplusplus/RawPacket.h>

TEST(DEFAULT, DefiningOfTypeHttpPacket)
{
    pstat::HttpPacketStat stat;


    pcpp::PcapFileReaderDevice reader("http-packets.pcap");
    if (!reader.open())
    {
        std::cerr << "Error opening the pcap file" << std::endl;
    }

    pcpp::RawPacket raw_packet;

    pcpp::RawPacketVector raw_vector;

    reader.getNextPackets(raw_vector);

    std::cout << raw_vector.size() << '\n';

    if (reader.getNextPacket(raw_packet))
    {
        pcpp::Packet parsed_packet(&raw_packet);
        stat.consume_packet(&parsed_packet);
    }

    reader.close();

    EXPECT_EQ(596, stat.get_count_http_packet());

}