#include "Defination.h"

#include <Nevermore/Net/Channel.h>
#include <Nevermore/Net/Acceptor.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <thread>
#include <utility>
#include <vector>
#include <algorithm>


using Channel = typename SF::IPv4TcpAcceptor::ChannelType;

std::vector<std::thread> threads{};

void worker(Channel channel);

int main()
{
    SF::IPv4TcpAcceptor acceptor{ EchoPort };

    while (true) {
        auto channel = acceptor.accept();
        std::thread t{ worker, std::move(channel) };
        threads.push_back(std::move(t));
    }

    for (auto&& t: threads)
        if (t.joinable())
            t.join();

    return EXIT_SUCCESS;
}

void worker(Channel channel)
{
    bool active = true;
    auto peer_addr = channel.peer_address().to_string();

    SPDLOG_INFO("{}", channel.self_address().to_string());

    while (active) {
        std::string request = channel.receive(MaxLine);
        SPDLOG_INFO("received from {}: {}", peer_addr, request);

        if (std::equal(request.cbegin(), request.cend(), Quit, Quit + 4)) {
            active = false;
            channel.send("Bye!");
        }
        else {
            channel.send(request);
        }
    }
}