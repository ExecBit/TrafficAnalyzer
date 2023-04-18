#include "http_packet_stat.hpp"

struct option analyzer_options[] =
{
    {"help", no_argument, 0, 'h'},
    {"interface", required_argument, 0, 'i'},
    {"period", required_argument, 0, 'p'},
    {"devices", no_argument, 0, 'd'},
    {0, 0, 0, 0},
};

void app_interrupted(void* cookie)
{
    bool* should_stop = (bool*) cookie;

    *should_stop = true;
}

void list_devices(log4cplus::Logger& logger)
{
    const std::vector<pcpp::PcapLiveDevice*>& list_interfaces = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

    if (list_interfaces.empty())
    {
        LOG4CPLUS_WARN(logger, LOG4CPLUS_TEXT("Not found network interfaces"));
        return;
    }

    std::cout << "Network devices:\n";

    for (auto i = list_interfaces.begin(); i != list_interfaces.end(); ++i)
    {
        std::cout << "Name: " << (*i)->getName() << "\tIPv4 adress: " << (*i)->getIPv4Address().toString() << '\n';

    }

    return;
}

void capture_http_packet(pcpp::RawPacket* raw_packet, pcpp::PcapLiveDevice* dev, void* data)
{
    pstat::HttpPacketStat* packet_stat = (pstat::HttpPacketStat*)data;

    pcpp::Packet parsed_packet(raw_packet);

    packet_stat->consume_packet(&parsed_packet);
}

void set_capture_device(log4cplus::Logger& logger, pcpp::PcapLiveDevice* dev, int output_period, bool* should_stop)
{
    //check device to open
    if (!dev->open())
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Could't open a device"));

    //set a filter
    pcpp::ProtoFilter httpFilter(pcpp::HTTP);

    if (!dev->setFilter(httpFilter))
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Could't set up on filter"));


    //start capture
    pstat::HttpPacketStat packet_stat;

    dev->startCapture(capture_http_packet, &packet_stat);

    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Start capture..."));

    while (!*should_stop)
    {
        pcpp::multiPlatformSleep(output_period);

        packet_stat.print_stat();
    }

    dev->stopCapture();
    dev->close();

    LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Stop capture!"));

    packet_stat.print_stat();
}

void print_help()
{
    std::cout << "Options:\n" << 
        "\t-i interface : Use the specified interface. Can be name (eth0) or IPv4 address\n" <<
        "\t-h help      : Displays help message\n" <<
        "\t-p period    : Use the specified value of period print of output\n" <<
        "\t-d devices   : Printed list name of network devices\n";
}

int main(int argc, char* argv[])
{
    //initializing application by PcapPlusPlus
    pcpp::AppName::init(argc, argv);

    //initializing logger (log4cplus)
    log4cplus::Initializer initializer;

    log4cplus::helpers::Properties props("../config/log4cplus-config.ini");
    props.setProperty(LOG4CPLUS_TEXT("log4cplus.appender.MyFileAppender.File"), LOG4CPLUS_TEXT("log.txt"));
    log4cplus::PropertyConfigurator conf(props);
    conf.configure();

    log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("main"));
    
    //handler application interrupted
    bool should_stop{false};
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted(app_interrupted, &should_stop);

    //basic parameters for capture device
    std::string interface_name;
    int output_period{5};

    //variable for getopt options
    int rez{0};
    int option_index{0};

    while ((rez = getopt_long(argc, argv, "h:i:p:d", analyzer_options, &option_index)) != -1)
    {
        switch (rez)
        {
            case 'h': print_help(); break;
            case 'i': interface_name = optarg; break;
            case 'p': output_period = atoi(optarg); break;
            case 'd': list_devices(logger); break;
            default: print_help(); return 0;
        }
    }

    pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(interface_name);
    if (dev == nullptr)
    {
        LOG4CPLUS_FATAL(logger, LOG4CPLUS_TEXT("Cound't find interface"));
        return -1;
    }

    set_capture_device(logger, dev, output_period, &should_stop);

    return 0;
}