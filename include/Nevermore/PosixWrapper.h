#ifndef NEVERMORE_POSIX_WRAPPER_H
#define NEVERMORE_POSIX_WRAPPER_H

#include <spdlog/spdlog.h>

#include <functional>
#include <type_traits>
#include <utility>
#include <system_error>
#include <cerrno>
#include <cstring>


namespace SF {

template<typename>
class PosixWrapper;


template<typename R, typename... Args>
class PosixWrapper<R(Args...)> {
public:
    using FuncType = std::function<R(Args...)>;

    R operator()(Args... args)
    {
        SPDLOG_TRACE("{} start");

        R result = func_(std::forward<Args>(args)...);
        check_result(result);

        SPDLOG_TRACE("{} end with result {}", );
        return result;
    }

private:
    std::string name_;
    FuncType func_;
    static std::string start_log_message_;

    void check_result(int res)
    {
        if (res == -1) {
            SPDLOG_ERROR("{} error: {}", name_, std::strerror(errno));
            throw std::system_error{ errno, 
                                     std::generic_category(), 
                                     fmt::format("{} error", name_) };
        }
    }
};

} // namespace SF

#endif // NEVERMORE_POSIX_WRAPPER_H