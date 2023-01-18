#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <Nevermore/PosixWrapper.h>

#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>


int main()
{
    auto fd = SF::Posix::create("test.file", S_IRWXU | S_IRWXG);
    std::string buffer{ "this is a test file" };
    SF::Posix::write(fd, buffer.c_str(), buffer.size());
    SF::Posix::close(fd);

    return EXIT_SUCCESS;
}