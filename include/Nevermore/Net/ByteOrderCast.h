#ifndef NEVERMORE_NET_BYTE_ORDER_CAST_H
#define NEVERMORE_NET_BYTE_ORDER_CAST_H

#include <endian.h>
#include <cstdint>


namespace SF {

// 本机字节序
struct Host;
// 网络字节序，大端
struct Net;

template<typename To>
struct ByteOrderCast {
    template<typename Integral>
    Integral operator()(Integral value)
    {
        return value;
    }
};

template<>
struct ByteOrderCast<Net> {
    constexpr char operator()(char value) const noexcept
    {
        return value;
    }

    constexpr std::uint8_t operator()(std::uint8_t value) const noexcept
    {
        return value;
    }
    
    std::uint16_t operator()(std::uint16_t value) const noexcept
    {
        return htobe16(value);
    }

    std::uint32_t operator()(std::uint32_t value) const noexcept
    {
        return htobe32(value);
    }

    std::uint64_t operator()(std::uint64_t value) const noexcept
    {
        return htobe64(value);
    }
};

template<>
struct ByteOrderCast<Host> {
    constexpr char operator()(char value) const noexcept
    {
        return value;
    }

    constexpr std::uint8_t operator()(std::uint8_t value) const noexcept
    {
        return value;
    }

    std::uint16_t operator()(std::uint16_t value) const noexcept 
    {
        return be16toh(value);
    }

    std::uint32_t operator()(std::uint32_t value) const noexcept
    {
        return be32toh(value);
    } 

    std::uint64_t operator()(std::uint64_t value) const noexcept
    {
        return be64toh(value);
    }
}; 

} // namespace SF

#endif // NEVERMORE_NET_BYTE_ORDER_CAST_H