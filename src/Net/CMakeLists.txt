
add_library(NevermoreNet
    STATIC
        ${SF_INTERNAL_INCLUDE_DIR}/Net/IP.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/ByteOrderCast.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/IPAddress.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/TCP.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/UDP.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/Connector.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/Channel.h
        ${SF_INTERNAL_INCLUDE_DIR}/Net/Acceptor.h
        IP.cpp
)

add_library(Nevermore::Net ALIAS NevermoreNet)

target_include_directories(NevermoreNet PRIVATE ${SF_INTERNAL_INCLUDE_DIR}/Net/)
target_include_directories(NevermoreNet PRIVATE ${SF_INTERNAL_INCLUDE_DIR}/)
target_include_directories(NevermoreNet PUBLIC ${SF_PUBLIC_INCLUDE_DIR})

target_link_libraries(NevermoreNet
    PRIVATE
        fmt::fmt
        spdlog::spdlog
)