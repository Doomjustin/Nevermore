#include "IP.h"
#include "ByteOrderCast.h"
#include "PosixWrapper.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <system_error>


namespace SF {

static constexpr const char* LocalHost = "localhost";
static constexpr const char* LocalHostIPv4Address = "127.0.0.1";
static constexpr const char* LocalHostIPv6Address = "::1";


sockaddr_in IPv4::init(const std::string_view address, PortType port)
{
    sockaddr_in result{};
    if (std::equal(address.cbegin(), address.cend(), 
                   LocalHost, LocalHost + 9))
        Posix::inet_pton(AF_INET, LocalHostIPv4Address, &result.sin_addr);
    else
        Posix::inet_pton(AF_INET, address.data(), &result.sin_addr);

    result.sin_family = AF_INET;
    result.sin_port = ByteOrderCast<Net>{}(port);

    return result;
}

sockaddr_in IPv4::init(PortType port)
{
    sockaddr_in result{};
    result.sin_family = AF_INET;
    result.sin_addr.s_addr = ByteOrderCast<Net>{}(INADDR_ANY);
    result.sin_port = ByteOrderCast<Net>{}(port);

    return result;
}

std::string IPv4::to_string(const AddressType& address)
{
    char buffer[INET_ADDRSTRLEN];

    Posix::inet_ntop(AF_INET, 
                    &address.sin_addr, 
                    buffer, 
                    sizeof(sockaddr_in));

    std::stringstream ss{};
    ss << "IPv4 Addresss["
       << buffer << ": "
       << ByteOrderCast<Host>{}(address.sin_port)
       << "]";

    return ss.str();  
}



sockaddr_in6 IPv6::init(const std::string_view address, PortType port)
{
    sockaddr_in6 result{};
    if (std::equal(address.cbegin(), address.cend(), 
                   LocalHost, LocalHost + 9))
        Posix::inet_pton(AF_INET6, LocalHostIPv6Address, &result.sin6_addr);
    else    
        Posix::inet_pton(AF_INET6, address.data(), &result.sin6_addr);

    result.sin6_family = AF_INET6;
    result.sin6_port = ByteOrderCast<Net>{}(port);

    return result;
}

sockaddr_in6 IPv6::init(PortType port)
{
    sockaddr_in6 result{};
    result.sin6_family = AF_INET6;
    result.sin6_addr = in6addr_any;
    result.sin6_port = ByteOrderCast<Net>{}(port);

    return result;    
}

std::string IPv6::to_string(const AddressType& address)
{
    std::stringstream ss{};

    char buffer[INET_ADDRSTRLEN];
    Posix::inet_ntop(AF_INET6, 
                    &address.sin6_addr, 
                    buffer, 
                    INET_ADDRSTRLEN);

    ss << "IPv6 Addresss["
       << buffer << ": "
       << ByteOrderCast<Host>{}(address.sin6_port)
       << "]";

    return ss.str();  
}

} // namespace SF