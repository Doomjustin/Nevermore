#ifndef NEVERMORE_LOG_SPDLOG_LOGGER_H
#define NEVERMORE_LOG_SPDLOG_LOGGER_H

#include "nevermore/Logger.h"

#include <spdlog/spdlog.h>
#include <memory>

namespace sf::log {

class SpdLogger: public Logger {
public:
    explicit SpdLogger(std::string_view name);

    void error(std::string_view message) noexcept override;
    void warning(std::string_view message) noexcept override;
    void debug(std::string_view message) noexcept override;
    void info(std::string_view message) noexcept override;
    void trace(std::string_view message) noexcept override;

    void level(LogLevel level) noexcept override;
    void pattern(const std::string& fmt) noexcept override;
    
private:
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace sf::log

#endif // NEVERMORE_LOG_SPDLOG_LOGGER_H