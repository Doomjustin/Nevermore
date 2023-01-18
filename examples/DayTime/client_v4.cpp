#include "Defination.h"

#include <Nevermore/PosixWrapper.h>

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>



int main(int argc, char* argv[])
{
    if (argc != 2)
        throw "Usage: day-time-client [IP Address]";

    auto sock_fd = SF::Posix::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DayTimePort);

    SF::Posix::inet_pton(AF_INET, argv[1], &server_address.sin_addr);

    SF::Posix::connect(sock_fd, (sockaddr*) &server_address, sizeof(server_address));

    ssize_t n;
    char buffer[MaxLine];
    while ((n = SF::Posix::read(sock_fd, buffer, MaxLine)) > 0) {
        buffer[n] = '\n';
        buffer[n + 1] = 0;

        if (fputs(buffer, stdout) == EOF)
            throw "fputs error";
    }

    return EXIT_SUCCESS;
}