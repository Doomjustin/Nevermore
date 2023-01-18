#ifndef NEVERMORE_POSIX_WRAPPER_H
#define NEVERMORE_POSIX_WRAPPER_H

#include <spdlog/spdlog.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <functional>
#include <type_traits>
#include <utility>
#include <system_error>
#include <cerrno>
#include <cstring>


namespace SF {

namespace detail {

template<size_t N>
struct LogHelper {
    static constexpr const char* value = "{} (...) start";
};

template<>
struct LogHelper<0> {
    static constexpr const char* value = "{} () start";
};

template<>
struct LogHelper<1> {
    static constexpr const char* value = "{} ({}) start";
};

template<>
struct LogHelper<2> {
    static constexpr const char* value = "{} ({}, {}) start";
};

template<>
struct LogHelper<3> {
    static constexpr const char* value = "{} ({}, {}, {}) start";
};

template<>
struct LogHelper<4> {
    static constexpr const char* value = "{} ({}, {}, {}, {}) start";
};

template<std::size_t N>
static inline constexpr const char* log_helper_v = LogHelper<N>::value;

} // namespace detail


// Posix函数的装饰器，在执行前后打印log，并检查执行结果是否正确，如果出错，则throw exception
template<typename>
class PosixWrapper;

template<typename R, typename... Args>
class PosixWrapper<R(Args...)> {
public:
    using FuncType = std::function<R(Args...)>;

public:
    template<typename Func>
        requires std::is_invocable_v<Func, Args...>
    PosixWrapper(const std::string& name, Func func)
      : name_{ name },
        func_{ func }
    {}

    R operator()(Args... args)
    {
        if constexpr (sizeof...(Args) <= 4)
            SPDLOG_INFO(detail::log_helper_v<sizeof...(Args)>, name_, args...);
        else
            SPDLOG_INFO("{} start", name_);

        R result = func_(std::forward<Args>(args)...);
        check_result(result);

        SPDLOG_INFO("{} end with result {}", name_, result);
        return result;
    }

private:
    std::string name_;
    FuncType func_;

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

template<typename R, typename... Args>
PosixWrapper<R(Args...)> make_posix_wrapper(R (*func)(Args...), const std::string& name)
{
    // 确保为全局唯一
    static auto instance = PosixWrapper<R(Args...)>{ name, func };
    return instance;
}

} // namespace SF


// 预定义的posix wrapper实例
namespace SF::Posix {

    static auto create = make_posix_wrapper(::creat, "create");
    static auto write = make_posix_wrapper(::write, "write");
    static auto read = make_posix_wrapper(::read, "read");
    static auto open = SF::PosixWrapper<int(const char*, int)>{ "open", ::open };
    static auto open_with_mode = SF::PosixWrapper<int(const char*, int, mode_t)>{ "open", ::open };
    static auto close = make_posix_wrapper(::close, "close");
    static auto lseek = make_posix_wrapper(::lseek, "lseek");
    static auto socket = make_posix_wrapper(::socket, "socket");
    static auto bind = make_posix_wrapper(::bind, "bind");
    static auto accept = make_posix_wrapper(::accept, "accept");
    static auto listen = make_posix_wrapper(::listen, "listen");
    static auto fork = make_posix_wrapper(::fork, "fork");
    static auto connect = make_posix_wrapper(::connect, "connect");

};

#endif // NEVERMORE_POSIX_WRAPPER_H