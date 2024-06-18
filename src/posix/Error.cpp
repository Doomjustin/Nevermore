#include "nevermore/posix/Error.h"
#include <format>


namespace sf::posix {

void check_reuslt(int res, std::source_location location)
{
    if (res == InvalidResult) {
        auto error_code = errno;
        auto error_msg = std::format("{}: {}", location.function_name(), strerror(error_code));
        std::system_error{ error_code, std::generic_category(), error_msg.c_str() };
    }
}

} // namespace sf::posix