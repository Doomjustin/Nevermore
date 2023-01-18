#ifndef NEVERMORE_NET_TCP_H
#define NEVERMORE_NET_TCP_H

#include "IP.h"
#include "IPAddress.h"
#include "../PosixWrapper.h"

#include <string_view>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <utility>


namespace SF {

template<typename IP>
class BasicTCP {
public:
    using AddressType = BasicIPAddress<IP>;
    using AddressReference = AddressType&;
    using ConstAddressReference = const AddressType&;
    using DomainType = typename IP::DomainType;
    using TypeType = decltype(SOCK_STREAM);

public:
    static constexpr DomainType domain = IP::domain;
    static constexpr TypeType type = SOCK_STREAM;

public:
    explicit BasicTCP(int fd)
      : fd_{ fd }
    {}

    BasicTCP(int fd, ConstAddressReference self_address)
      : fd_{ fd },
        self_address_{ self_address }
    {}

    BasicTCP(int fd, ConstAddressReference self_address, ConstAddressReference peer_address)
      : fd_{ fd },
        self_address_{ self_address },
        peer_address_{ peer_address }
    {}

    BasicTCP(const BasicTCP& other)
      : fd_{ Posix::duplicate(other.fd_) },
        self_address_{ other.self_address_ },
        peer_address_{ other.peer_address_ }
    {}   

    BasicTCP& operator<<(const BasicTCP& other)
    {
        BasicTCP tmp{ other };

        std::swap(tmp, *this);

        return *this;
    }

    BasicTCP(BasicTCP&& other) noexcept
      : fd_{ std::move(other.fd_) },
        self_address_{ std::move(other.self_address_) },
        peer_address_{ std::move(other.peer_address_) }
    {
        other.fd_ = -1;
    }

    BasicTCP& operator=(BasicTCP&& other) noexcept
    {
        std::swap(*this, other);

        return *this;
    }

    ~BasicTCP() 
    {
        if (fd_ != -1) {
            Posix::close(fd_);
            fd_ = -1;
        }
    }


    void send(const std::string_view message)
    {
        Posix::write(fd_, message.data(), message.size());
    }

    std::string receive(std::size_t count)
    {
        char buffer[count];
        auto read_bytes = Posix::read(fd_, buffer, count);
        buffer[read_bytes] = 0;
        
        std::stringstream ss{};
        ss << buffer;    

        return ss.str();       
    }

    constexpr int fd() const noexcept { return fd_; }

    constexpr bool valid() const noexcept { return fd_ == -1; }

    AddressReference self_address() noexcept { return self_address_; }

    AddressReference peer_address() noexcept { return peer_address_; }

private:
    int fd_;
    AddressType self_address_;
    AddressType peer_address_;
};

using IPv4TCP = BasicTCP<IPv4>;
using IPv6TCP = BasicTCP<IPv6>;

} // namespace SF

#endif // NEVERMORE_NET_TCP_H