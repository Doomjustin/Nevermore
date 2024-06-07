#ifndef NEVERMORE_DATABASE_STATUS_H
#define NEVERMORE_DATABASE_STATUS_H

#include <cstdint>
#include <string_view>

namespace sf {

/**
 * @brief A Status encapsulates the result of an operation.
 * It may indicate success, or it may indicate an error with an associated error message.
 * Multiple threads can invoke const methods on a Status without external synchronization, 
 * but if any of the threads may call a non-const method, 
 * all threads accessing the same Status must use external synchronization.
 * 
 */
class Status {
public:
    /**
     * @brief Create a success status
     * 
     */
    Status();

    Status(const Status& rhs);
    Status& operator=(const Status& rhs);

    Status(Status&& rhs);
    Status& operator=(Status&& rhs) noexcept;

    ~Status();

    static Status ok();
    static Status not_found(const std::string_view msg, const std::string_view msg2 = std::string_view{});
    static Status corruption(const std::string_view msg, const std::string_view msg2 = std::string_view{});
    static Status not_supported(const std::string_view msg, const std::string_view msg2 = std::string_view{});
    static Status invalid_argument(const std::string_view msg, const std::string_view msg2 = std::string_view{});    
    static Status io_error(const std::string_view msg, const std::string_view msg2 = std::string_view{});

    constexpr bool is_ok() const noexcept { return code() == Code::Ok; }
    constexpr bool is_not_found() const noexcept { return code() == Code::NotFound; }
    constexpr bool is_corruption() const noexcept { return code() == Code::Corruption; }
    constexpr bool is_io_error() const noexcept { return code() == Code::IOError; }
    constexpr bool is_not_supported() const noexcept { return code() == Code::NotSupported; }
    constexpr bool is_invalid_argument() const noexcept { return code() == Code::InvalidArgument; }

    std::string to_string() const noexcept;

private:
    enum class Code: std::uint8_t {
        Ok,
        NotFound,
        Corruption,
        NotSupported,
        InvalidArgument,
        IOError
    };

    Status(Code code, const std::string_view msg, const std::string_view msg2);

    constexpr Code code() const noexcept 
    {
        return state_ == nullptr ? Code::Ok : static_cast<Code>(state_[4]);
    }

    static const char* copy_state(const char* state);

    // OK status has a null state_.  Otherwise, state_ is a new[] array
    // of the following form:
    //    state_[0..3] == length of message
    //    state_[4]    == code
    //    state_[5..]  == message
    const char* state_;
};

} // namespace sf

#endif // NEVERMORE_DATABASE_STATUS_H