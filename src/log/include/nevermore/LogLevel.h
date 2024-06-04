#ifndef NEVERMORE_LOG_LEVEL_H
#define NEVERMORE_LOG_LEVEL_H

namespace sf {

enum class LogLevel: char {
    Off,
    Error,
    Warning,
    Debug,
    Info,
    Trace
};

} // namespace sf

#endif // NEVERMORE_LOG_LEVEL_H