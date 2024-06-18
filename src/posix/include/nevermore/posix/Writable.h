#ifndef NEVERMORE_POSIX_WRITABLE_H
#define NEVERMORE_POSIX_WRITABLE_H

#include <cstring>
#include <string>
#include <unistd.h>

#include "Error.h"

namespace sf::posix {

class WriteException: public PosixException<WriteException> {
public:
    WriteException(int err_code)
      : PosixException{ err_code }
    {}

    ~WriteException() = default;

    std::string name() const noexcept { return "WriteException"; }
};


template<typename T>
class Writable {
public:
    virtual ~Writable() = default;
 
    ssize_t write(const void* buf, std::size_t count)
    {
        auto res = ::write(derived().fd(), buf, count);
        check_result<WriteException>(res);

        return res;
    }

private:
    T& derived()
    {
        return static_cast<T&>(*this);
    }
};

} // namespace sf::posix

#endif // NEVERMORE_POSIX_WRITABLE_H