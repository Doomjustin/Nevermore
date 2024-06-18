#include "nevermore/posix/File.h"
#include "nevermore/posix/Error.h"

#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace sf::posix {

int File::create(const char* path, int flags, mode_t mode)
{
    auto fd = ::open(path, flags, mode);
    check_result<CreateFileException>(fd);
    return fd;
}

int File::open(const char* path, int flags)
{
    auto fd = ::open(path, flags);
    check_result<OpenFileException>(fd);
    return fd;
}


File::File(const char* path, int flags, mode_t mode)
  : File{ create(path, flags, mode) }
{}

File::File(const char* path, int flags)
  : File{ open(path, flags) }
{}

File::File(int fd)
  : fd_{ fd }
{}

File::~File()
{
    if (closable())
        close();
}

void File::close()
{
    assert(closable());
    auto res = ::close(fd());
    check_result<CloseFileException>(res);
}

} // namespace sf::posix