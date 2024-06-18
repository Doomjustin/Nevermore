#ifndef NEVERMORE_POSIX_ERROR_H
#define NEVERMORE_POSIX_ERROR_H

#include <concepts>
#include <cstring>
#include <exception>
#include <source_location>
#include <format>

namespace sf::posix {

template<typename Error>
class PosixException: public std::exception {
public:
    PosixException(int err_code)
      : err_code_{ err_code }
    {}

    ~PosixException() = default;

    const char* what() const noexcept override
    {
        return err_msg_.c_str();
    }

private:
    int err_code_;
    std::string err_msg_ = std::format("{}, {}", derived().name(), strerror(err_code_));

    const Error& derived() const noexcept
    {
        return static_cast<const Error&>(*this);
    }
};


static constexpr int InvalidResult = -1;

template<typename Error>
    requires std::derived_from<Error, PosixException<Error>>
void check_result(int res)
{
    if (res == InvalidResult)
        throw Error{ errno };
}

void check_result(int res, std::source_location location = std::source_location::current());

} // namespace sf::posix

#endif // NEVERMORE_POSIX_ERROR_H