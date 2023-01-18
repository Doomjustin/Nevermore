#include "Defination.h"

#include <Nevermore/Net/Connector.h>

#include <cstdlib>
#include <iostream>
#include <sys/socket.h>


int main(int argc, char* argv[])
{
    std::string address{};
    if (argc != 2)
        address = "localhost";
    else
        address = argv[1];

    SF::IPv6TcpConnector connector{};
    auto channel = connector.connect(address, DayTimePort);
    
    auto response = channel.receive(MaxLine);
    std::cout << response << std::endl;

    return EXIT_SUCCESS;
}