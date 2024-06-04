#include "nevermore/Logger.h"
#include "SpdLogger.h"

#include <memory>

namespace sf {

std::unique_ptr<Logger> make_logger(std::string_view name)
{
    return std::make_unique<log::SpdLogger>(name);
}

} // namespace sf