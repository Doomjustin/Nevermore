#ifndef NEVERMORE_NET_IP_H
#define NEVERMORE_NET_IP_H

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <string_view>


namespace SF {

struct IPv4 {
    using AddressType = sockaddr_in;
    using PortType = in_port_t;
    using LengthType = socklen_t;
    using DomainType = decltype(AF_INET);

    static constexpr DomainType domain = AF_INET;
    static std::string to_string(const AddressType& address);

    static AddressType init(const std::string_view address, PortType port = 0);
    static AddressType init(PortType port = 0);
};

struct IPv6 {
    using AddressType = sockaddr_in6;
    using PortType = in_port_t;
    using LengthType = socklen_t;
    using DomainType = decltype(AF_INET6);

    static constexpr DomainType domain = AF_INET6;
    static std::string to_string(const AddressType& address);
    
    static AddressType init(const std::string_view address, PortType port = 0);
    static AddressType init(PortType port = 0);
};

} // namespace SF

#endif // NEVERMORE_NET_IP_H