#ifndef NEVERMORE_NET_CHANNEL_H
#define NEVERMORE_NET_CHANNEL_H

#include <string>
#include <string_view>
#include <cstddef>


namespace SF {

template<typename TP>
class BasicChannel {
public:
    using AddressType = typename TP::AddressType;
    using AddressReference = AddressType&;
    using ConstAddressReference = const AddressType&;

public:
    BasicChannel(int fd)
      : tp_{ fd }
    {}

    BasicChannel(int fd, ConstAddressReference self_address)
      : tp_{ fd, self_address }
    {}
    
    BasicChannel(int fd, ConstAddressReference self_address, ConstAddressReference peer_address)
      : tp_{ fd, self_address, peer_address }
    {}

    ~BasicChannel() {}

    void send(const std::string_view message) { tp_.send(message); }

    std::string receive(std::size_t count) { return tp_.receive(count); }

    ConstAddressReference peer_address() const noexcept { return tp_.peer_address(); }
    
    AddressReference peer_address() noexcept { return tp_.peer_address(); }

    ConstAddressReference self_address() const noexcept { return tp_.self_address(); }

    AddressReference self_address() noexcept { return tp_.self_address(); }

private: 
    TP tp_;
};

} // namespace SF

#endif // NEVERMORE_NET_CHANNEL_H