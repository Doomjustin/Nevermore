#ifndef NEVERMORE_POSIX_READABLE_H
#define NEVERMORE_POSIX_READABLE_H

#include <cstddef>
#include <sys/types.h>
#include <unistd.h>

#include "Error.h"

namespace sf::posix {

class ReadException: public PosixException<ReadException> {
public:
    ReadException(int err_code)
      : PosixException{ err_code }
    {}
    
    ~ReadException() = default;

    std::string name() const noexcept { return "ReadException"; }
};


template<typename T>
class Readable {
public:
    virtual ~Readable() = default;

    ssize_t read(void* buf, size_t count)
    {
        auto res = ::read(derived().fd(), buf, count);
        check_result<ReadException>(res);

        return res;
    }

private:
    T& derived()
    {
        return static_cast<T&>(*this);
    }
};

} // namespace sf::posix

#endif // NEVERMORE_POSIX_READABLE_H