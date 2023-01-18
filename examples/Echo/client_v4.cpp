#include "Defination.h"

#include <Nevermore/Net/Connector.h>

#include <cstdlib>
#include <iostream>
#include <algorithm>


int main()
{
    SF::IPv4TcpConnector connector{};
    auto channel = connector.connect("localhost", EchoPort);

    bool active = true;
    
    std::cout << "Welcome Using Echo" << '\n';
    while (active) {
        std::string get_in;
        std::cin >> get_in;

        channel.send(get_in);
        
        if (std::equal(get_in.cbegin(), get_in.cend(), Quit, Quit + 4))
            active = false;

        std::string response = channel.receive(MaxLine);
        std::cout << response << '\n';
    }

    return EXIT_SUCCESS;
}