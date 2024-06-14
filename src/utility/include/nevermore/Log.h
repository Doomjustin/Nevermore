#ifndef NEVERMORE_UTILITY_LOG_H
#define NEVERMORE_UTILITY_LOG_H

#include <string_view>
#include <memory>
#include <format>

namespace sf {

enum class LogLevel: char {
    Off,
    Error,
    Warning,
    Debug,
    Info,
    Trace
};

class Logger {
public:
    virtual ~Logger() = default;

    virtual void error(std::string_view message) noexcept = 0;
    virtual void warning(std::string_view message) noexcept = 0;
    virtual void debug(std::string_view message) noexcept = 0;
    virtual void info(std::string_view message) noexcept = 0;
    virtual void trace(std::string_view message) noexcept = 0;

    constexpr LogLevel level() const noexcept { return level_; }
    virtual void level(LogLevel level) noexcept = 0;

    virtual void pattern(const std::string& std) noexcept = 0;

protected:
    LogLevel level_ = LogLevel::Debug;
};

std::unique_ptr<Logger> make_logger(std::string_view name);


namespace log {

Logger& default_logger();

void pattern(const std::string& std);
void level(LogLevel level);

template<typename... T>
void error(std::format_string<T...> std, T&&... args)
{
    default_logger().error(std::format(std, std::forward<T>(args)...));
}

template<typename... T>
void warning(std::format_string<T...> std, T&&... args)
{
    default_logger().warning(std::format(std, std::forward<T>(args)...));
}

template<typename... T>
void debug(std::format_string<T...> std, T&&... args)
{
    default_logger().debug(std::format(std, std::forward<T>(args)...));
}

template<typename... T>
void info(std::format_string<T...> std, T&&... args)
{
    default_logger().info(std::format(std, std::forward<T>(args)...));
}

template<typename... T>
void trace(std::format_string<T...> std, T&&... args)
{
    default_logger().trace(std::format(std, std::forward<T>(args)...));
}

} // namespace sf::log

} // namespace sf

#ifndef NDEBUG
    #define SF_ERROR(...) sf::log::error(__VA_ARGS__)
    #define SF_WARN(...)  sf::log::warning(__VA_ARGS__)
    #define SF_DEBUG(...) sf::log::debug(__VA_ARGS__)
    #define SF_INFO(...)  sf::log::info(__VA_ARGS__)
    #define SF_TRACE(...) sf::log::trace(__VA_ARGS__)
#else
    #define SF_ERROR(...)
    #define SF_WARN(...)
    #define SF_DEBUG(...)
    #define SF_INFO(...) 
    #define SF_TRACE(...)
#endif

#endif // NEVERMORE_UTILITY_LOG_H