#include "nevermore/Log.h"

namespace sf::log {

Logger& default_logger()
{
    static std::unique_ptr<Logger> default_logger_ = make_logger("Nevermore Logger");

    return *default_logger_;
}

void pattern(const std::string& fmt)
{
    default_logger().pattern(fmt);
}

void level(LogLevel level)
{
    default_logger().level(level);
}

} // namespace sf::log