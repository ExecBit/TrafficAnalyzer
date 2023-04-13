#include <getopt.h>
#include <log4cplus/tchar.h>
#include <pcapplusplus/Device.h>
#include <pcapplusplus/Packet.h>
#include <pcapplusplus/PcapFilter.h>
#include <pcapplusplus/ProtocolType.h>
#include <pcapplusplus/SystemUtils.h>
#include <pcapplusplus/PcapLiveDevice.h>
#include <pcapplusplus/PcapLiveDeviceList.h>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/initializer.h>

#include "http_packet_stat.hpp"

struct option analyzer_options[] =
{
    {"help", no_argument, 0, 'h'},
    {"interface", required_argument, 0, 'i'},
    {"period", required_argument, 0, 'p'},
    {0, 0, 0, 0},
};

void capture_http_packet(pcpp::RawPacket* raw_packet, pcpp::PcapLiveDevice* dev, void* data)
{
    HttpPacketStat* packet_stat = (HttpPacketStat*)data;

    pcpp::Packet parsed_packet(raw_packet);

    packet_stat->consume_packet(parsed_packet);
}

void set_capture_device(log4cplus::Logger& logger, pcpp::PcapLiveDevice* dev, int output_period)
{
    //check device to open
    if (!dev->open())
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Could't open a device"));

    //set a filter
    pcpp::ProtoFilter httpFilter(pcpp::HTTP);

    if (!dev->setFilter(httpFilter))
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Could't set up on filter"));


    //start capture
    HttpPacketStat packet_stat;

    dev->startCapture(capture_http_packet, &packet_stat);

    //set a timer
    bool shouldStop{false};

//  pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted(onApplicationInterrupted, &shouldStop);

    while (!shouldStop)
    {
        pcpp::multiPlatformSleep(output_period);

        packet_stat.print_current_stat();
    }

    dev->stopCapture();
    dev->close();

    packet_stat.print_final_stat();
}


void print_help()
{
    std::cout << "i - interface name\t h - help\n";
}


int main(int argc, char* argv[])
{
    //initializing application by PcapPlusPlus
    pcpp::AppName::init(argc, argv);

    //initializing logger (log4cplus)
    log4cplus::Logger logger;

    log4cplus::BasicConfigurator config;
    config.configure();

    logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    

    //basic parameters for capture device
    std::string interface_name;
    int output_period{5};

    //variable for getopt options
    int rez{0};
    int option_index{0};

    while ((rez = getopt_long(argc, argv, "h:i:p", analyzer_options, &option_index)) != -1)
    {
        switch (rez)
        {
            case 0 : break;
            case 'h': print_help(); break;
            case 'i': interface_name = optarg; break;
            case 'p': output_period = atoi(optarg); break;
            default: print_help(); return -1;
        }
    }

    pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(interface_name);
    if (dev == nullptr)
    {
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Cound't find interface"));
        return -1;
    }

    set_capture_device(logger, dev, output_period);

    return 0;
}