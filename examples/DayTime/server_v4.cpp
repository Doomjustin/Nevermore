#include "Defination.h"

#include <Nevermore/PosixWrapper.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <netinet/in.h>
#include <sys/socket.h>


int main(int argc, char* argv[])
{
    auto listen_fd = SF::Posix::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(DayTimePort);

    SF::Posix::bind(listen_fd, (sockaddr*) &server_addr, sizeof(server_addr));
    SF::Posix::listen(listen_fd, 5);

    int connected_fd;
    time_t ticks;
    char buffer[MaxLine];

    while (true) {
        connected_fd = SF::Posix::accept(listen_fd, nullptr, nullptr);
        ticks = std::time(nullptr);    

        snprintf(buffer, sizeof(buffer), "%.24s", std::ctime(&ticks));
        SF::Posix::write(connected_fd, buffer, strlen(buffer));
        SPDLOG_INFO("connected from {}, write back {}", connected_fd, buffer);

        SF::Posix::close(connected_fd);        
    }

    return EXIT_SUCCESS;
}