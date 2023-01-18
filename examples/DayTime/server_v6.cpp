#include "Defination.h"

#include <Nevermore/Net/Acceptor.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>


int main()
{
    SF::IPv6TcpAcceptor acceptor{ DayTimePort };

    time_t ticks;

    while (true) {
        auto channel = acceptor.accept();

        ticks = std::time(nullptr);    

        std::stringstream ss{};
        ss << std::asctime(std::localtime(&ticks));
        channel.send(ss.str());

        std::cout << channel.peer_address().to_string() << '\n';
    }

    return EXIT_SUCCESS;
}