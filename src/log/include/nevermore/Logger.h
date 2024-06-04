#ifndef NEVERMORE_LOG_LOGGER_H
#define NEVERMORE_LOG_LOGGER_H

#include <string_view>
#include <memory>

#include "LogLevel.h"

namespace sf {

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

} // namespace sf

#endif // NEVERMORE_LOG_LOGGER_H