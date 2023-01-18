#ifndef NEVERMORE_NET_IP_ADDRESS_H
#define NEVERMORE_NET_IP_ADDRESS_H

#include <string_view>
#include <string>


namespace SF {

template<typename IP>
class BasicIPAddress {
public:
    using PortType = typename IP::PortType;
    using LengthType = typename IP::LengthType;
    using DomainType = typename IP::DomainType;
    using AddressType = typename IP::AddressType;
    using AddressReference = AddressType&;    
    using ConstAddressReference = const AddressType&;    

public:
    static constexpr LengthType length() { return sizeof(AddressType); }
    static constexpr DomainType domain() { return IP::domain; }

public:
    explicit BasicIPAddress(const std::string_view address, PortType port = 0)
      : address_{ IP::init(address, port) }
    {}

    explicit BasicIPAddress(PortType port = 0)
      : address_{ IP::init(port) }
    {}

    AddressReference get() noexcept { return address_; }
    ConstAddressReference get() const noexcept { return address_; }

    std::string to_string() const noexcept
    {
        return IP::to_string(address_);
    }

private:
    AddressType address_;    
};

} // namespace SF

#endif // NEVERMORE_NET_IP_ADDRESS_H