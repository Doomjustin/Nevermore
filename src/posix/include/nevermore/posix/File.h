#ifndef NEVERMORE_POSIX_FILE_H
#define NEVERMORE_POSIX_FILE_H

#include <unistd.h>
#include <fcntl.h>

#include "Writable.h"
#include "Readable.h"
#include "Error.h"

namespace sf::posix {

class CreateFileException: public PosixException<CreateFileException> {
public:
    CreateFileException(int err_code)
      : PosixException{ err_code }
    {}

    ~CreateFileException() = default;

    std::string name() const noexcept { return "CreateFileException"; }
};

class OpenFileException: public PosixException<OpenFileException> {
public:
    OpenFileException(int err_code)
      : PosixException{ err_code }
    {}

    ~OpenFileException() = default;

    std::string name() const noexcept { return "OpenFileException"; }
};

class CloseFileException: public PosixException<CloseFileException> {
public:
    CloseFileException(int err_code)
      : PosixException{ err_code }
    {}

    ~CloseFileException() = default;

    std::string name() const noexcept { return "CloseFileException"; }
};


// Regular file
class File: public Writable<File>,
            public Readable<File> {
public:
    static constexpr int InvalidFd = -1;

    File(const char* path, int flags, mode_t mode);
    File(const char* path, int flags);
    File(int fd);

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&&) noexcept = default;
    File& operator=(File&&) noexcept = default;

    ~File();

    constexpr int fd() const noexcept { return fd_; }
 
    constexpr bool closable() const noexcept { return fd_ != InvalidFd; }

    void close();

private:
    int fd_;

    static int create(const char* path, int flags, mode_t mode);
    
    static int open(const char* path, int flags);
};

} // namespace sf::posix

#endif // NEVERMORE_POSIX_FILE_H