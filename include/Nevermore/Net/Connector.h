#ifndef NEVERMORE_NET_CONNECTOR_H
#define NEVERMORE_NET_CONNECTOR_H

#include "TCP.h"
#include "Channel.h"
#include "../PosixWrapper.h"


namespace SF {

template<typename TP>
class BasicConnector {
public:    
    using AddressType = typename TP::AddressType;
    using AddressReference = AddressType&;
    using ConstAddressReference = const AddressType&;
    using ChannelType = BasicChannel<TP>;
    using PortType = typename AddressType::PortType;

public:
    BasicConnector()
      : fd_{ Posix::socket(TP::domain, TP::type, 0) }
    {}

    ~BasicConnector()
    {
        if (fd_ != -1)
            Posix::close(fd_);
    }

    ChannelType connect(AddressType target_address)
    {
        Posix::connect(fd_, 
                      (sockaddr*) &target_address.get(),
                      target_address.length());

        ChannelType channel{ fd_ };
        channel.peer_address() = std::move(target_address);
        fd_ = -1;

        return channel;
    }

    ChannelType connect(const std::string_view addr, PortType port)
    { 
        AddressType peer_address{ addr, port };
        return connect(std::move(peer_address));
    }

private:
    int fd_;
};

using IPv4TcpConnector = BasicConnector<IPv4TCP>;
using IPv6TcpConnector = BasicConnector<IPv6TCP>;

} // namespace SF

#endif // NEVERMORE_NET_CONNECTOR_H