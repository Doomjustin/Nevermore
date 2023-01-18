#ifndef NEVERMORE_NET_ACCEPTOR_H
#define NEVERMORE_NET_ACCEPTOR_H

#include "TCP.h"
#include "Channel.h"
#include "../PosixWrapper.h"


namespace SF {

static constexpr int BackQueue = 5;

template<typename TP>
class BasicAcceptor {
public:
    using AddressType = typename TP::AddressType;
    using AddressReference = AddressType&;
    using ConstAddressReference = const AddressType&;
    using ChannelType = BasicChannel<TP>;
    using PortType = typename AddressType::PortType;

public:
    explicit BasicAcceptor(ConstAddressReference self_address, int back_log = BackQueue)
      : fd_{ Socket(TP::domain, TP::type, 0) },
        self_address_{ self_address }
    {
        Posix::bind(fd_, 
                    (sockaddr*) &self_address_.get(), 
                    AddressType::length());
        Posix::listen(fd_, back_log);
    }

    explicit BasicAcceptor(const std::string_view addr, PortType port = 0, int back_log = BackQueue)
      : fd_{ Posix::socket(TP::domain, TP::type, 0) },
        self_address_{ addr, port }
    {
        Posix::bind(fd_, 
                    (sockaddr*) &self_address_.get(), 
                    AddressType::length());
        Posix::listen(fd_, back_log);
    }

    explicit BasicAcceptor(PortType port = 0, int back_log = BackQueue)
      : fd_{ Posix::socket(TP::domain, TP::type, 0) },
        self_address_{ port }
    {
        Posix::bind(fd_, 
                    (sockaddr*) &self_address_.get(), 
                    AddressType::length());
        Posix::listen(fd_, back_log);
    }

    BasicAcceptor(const BasicAcceptor& other)
      : fd_{ Posix::duplicate(other.fd_) },
        self_address_{ other.self_address_ }
    {}

    BasicAcceptor& operator=(const BasicAcceptor& other)
    {
        BasicAcceptor tmp{ other };

        std::swap(tmp, *this);

        return this;
    }

    BasicAcceptor(BasicAcceptor&& other) noexcept
      : fd_{ std::move(other.fd_) },
        self_address_{ std::move(other.self_address_) }
    {
        other.fd_ = -1;
    }

    BasicAcceptor& operator=(BasicAcceptor&& other) noexcept
    {
        std::swap(*this, other);

        return *this;
    }

    ~BasicAcceptor()
    {
        if (fd_ != -1) {
            Posix::close(fd_);
            fd_ = -1;
        }
    }

    ChannelType accept()
    {
        AddressType peer_address{};
        typename AddressType::LengthType length = 0;

        auto accepted_fd = Posix::accept(fd_, 
                                        (sockaddr*) &peer_address.get(), 
                                        &length);

        ChannelType channel{ accepted_fd, self_address_, peer_address };
        return channel;
    }

public:
    int fd_;
    AddressType self_address_;
};

using IPv4TcpAcceptor = BasicAcceptor<IPv4TCP>;
using IPv6TcpAcceptor = BasicAcceptor<IPv6TCP>;

} // namespace SF

#endif // NEVERMORE_NET_ACCEPTOR_H