#include "nevermore/Status.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

namespace sf {

Status::Status()
  : state_{ nullptr }
{}

Status::Status(Code code, const std::string_view msg, const std::string_view msg2)
{
    assert(code != Code::Ok);
    const uint32_t len1 = static_cast<uint32_t>(msg.size());
    const uint32_t len2 = static_cast<uint32_t>(msg2.size());
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
    char* result = new char[size + 5];
    std::memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    std::memcpy(result + 5, msg.data(), len1);
    if (len2) {
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        std::memcpy(result + 7 + len1, msg2.data(), len2);
    }
    state_ = result;
}

Status::Status(const Status& rhs)
{
    state_ = rhs.state_ == nullptr ? nullptr : copy_state(rhs.state_);
}

Status& Status::operator=(const Status& rhs)
{
    if (state_ != rhs.state_) {
        delete[] state_;
        state_ = rhs.state_ == nullptr ? nullptr : copy_state(rhs.state_);
    }

    return *this;
}

Status::Status(Status&& rhs)
  : state_{ rhs.state_ }
{
    rhs.state_ = nullptr;
}
 
Status& Status::operator=(Status&& rhs) noexcept
{
    std::swap(state_, rhs.state_);
    return *this;
}

Status::~Status()
{
    delete[] state_;
}

Status Status::ok() 
{ 
    return Status{}; 
}

Status Status::not_found(const std::string_view msg, const std::string_view msg2) 
{
    return Status{ Code::NotFound, msg, msg2 };
}

Status Status::corruption(const std::string_view msg, const std::string_view msg2) 
{
    return Status{ Code::Corruption, msg, msg2 };
}

Status Status::not_supported(const std::string_view msg, const std::string_view msg2) 
{
    return Status{ Code::NotSupported, msg, msg2 };
}

Status Status::invalid_argument(const std::string_view msg, const std::string_view msg2) 
{
    return Status{ Code::InvalidArgument, msg, msg2 };
}

Status Status::io_error(const std::string_view msg, const std::string_view msg2) 
{
    return Status{ Code::IOError, msg, msg2 };
}

std::string Status::to_string() const noexcept
{
    if (state_ == nullptr) {
        return "OK";
    } 
    else {
        char tmp[30];
        const char* type;
        switch (code()) {
        using enum Code;
        case Ok:
            type = "OK";
            break;
        case NotFound:
            type = "NotFound: ";
            break;
        case Corruption:
            type = "Corruption: ";
            break;
        case NotSupported:
            type = "Not implemented: ";
            break;
        case InvalidArgument:
            type = "Invalid argument: ";
            break;
        case IOError:
            type = "IO error: ";
            break;
        default:
            std::snprintf(tmp, sizeof(tmp),
                        "Unknown code(%d): ", static_cast<int>(code()));
            type = tmp;
            break;
        }
        std::string result { type };
        uint32_t length;
        std::memcpy(&length, state_, sizeof(length));
        result.append(state_ + 5, length);
        return result;
    }
}

const char* Status::copy_state(const char* state)
{
    uint32_t size;
    std::memcpy(&size, state, sizeof(size));
    char* result = new char[size + 5];
    std::memcpy(result, state, size + 5);
    return result;
}

} // namespace sf