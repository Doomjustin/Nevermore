#ifndef NEVERMORE_LOG_LOG_H
#define NEVERMORE_LOG_LOG_H

#include <format>

#include "Logger.h"

namespace sf::log {

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

#if defined (NEVERMORE_DEBUG)
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

#endif // NEVERMORE_LOG_LOG_H