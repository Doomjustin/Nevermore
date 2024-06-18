#ifndef NEVERMORE_LEVELDB_VARINT_H
#define NEVERMORE_LEVELDB_VARINT_H

#include <cstdint>
#include <istream>
#include <iterator>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>

namespace sf {

/**
 * @brief value在编码为二进制时，会自动根据值的大小调节使用的byte数
 * 
 * 大体上，想定的使用对象是std::uint32_t和std::uint64_t，其他的类型没有测试
 *
 * @tparam T 
 */
template<typename T>
    requires std::is_unsigned_v<T>
class Varint {
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Varint<U> value);

public:
    Varint(T value)
      : value_{ value }
    {}

    Varint(const std::string& value)
      : value_{ decode(value) }
    {}

    constexpr T value() const noexcept { return value_; }

    /**
     * @brief 编码成二进制形式
     * 
     * 每个byte的第一位作为指示位，表示是否是最后的存储byte，剩下7位实际存储值的编码
     * 0bxxxxxxxx
     *   ^
     *   |
     *   当这个bit为1时，则表示当前byte无法完全存储所有值，需要继续往前寻找一个byte解码
     *   当这个bit为0时，则表示当前byte是最后的存储值byte，无需继续往前解码
     * @param os 
     */
    void encode(std::ostream& os) const
    {
        constexpr auto BufSize = std::is_same_v<T, std::uint32_t> ? 5 : 10;
        char buf[BufSize]{};
        auto ptr = encode(buf, value_);
        os.write(buf, ptr - buf);
    }

private:
    T value_;
    static constexpr auto ByteSize = std::numeric_limits<unsigned char>::digits;
    static constexpr auto VarintByteSize = ByteSize - 1;
    static constexpr auto VarintMaxByteValue = 1 << 7;
    
    static char* encode(char* dst, T value)
    {
        auto* ptr = reinterpret_cast<std::uint8_t*>(dst);

        while (value >= VarintMaxByteValue) {
            *(ptr++) = value | VarintMaxByteValue;
            value >>= VarintByteSize;
        }
        *(ptr++) = static_cast<std::uint8_t>(value);
        return reinterpret_cast<char*>(ptr);
    }

    /**
     * @brief 解码value中的varint为T类型的value
     * 
     * @param value 
     * @return T 
     * @pre value刚好包含要decode的二进制编码值，
     * 且value.size()一定是ByteSize的整数倍，
     * 且value的表示是小端表示
     */
    static T decode(const std::string& value)
    {
        std::string encoded_value{};

        // value.size()一定是ByteSize的整数倍
        bool is_not_last_byte = true;
        auto flag = std::prev(value.end(), ByteSize);
        auto start = std::next(flag, 1);
        auto end = value.end();

        while (is_not_last_byte) {
            encoded_value.insert(encoded_value.begin(), start, end);
            if (*flag == '0') {
                is_not_last_byte = false;
                break;
            }

            end = flag;
            flag = std::prev(flag, ByteSize);
            start = std::next(flag, 1);
        }

        if constexpr (std::is_same_v<T, std::uint32_t>)
            return std::stoul(encoded_value, nullptr, 2);
        else
            return std::stoull(encoded_value, nullptr, 2);
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Varint<T> value)
{
    value.encode(os);
    return os;
}

template<typename T>
Varint<T> parse(std::istream& is);

Varint(char) -> Varint<std::uint32_t>;
Varint(unsigned char) -> Varint<std::uint32_t>;

Varint(short) -> Varint<std::uint32_t>;
Varint(unsigned short) -> Varint<std::uint32_t>;

Varint(int) -> Varint<std::uint32_t>;

static constexpr auto LongSize = sizeof(long);
#if (LongSize == 4)
Varint(unsigned long) -> Varint<std::uint32_t>;
Varint(long) -> Varint<std::uint32_t>;
#else
Varint(unsigned long) -> Varint<std::uint64_t>;
Varint(long) -> Varint<std::uint64_t>;
#endif

Varint(long long) -> Varint<std::uint64_t>;

Varint(const std::string&) -> Varint<std::uint32_t>;

} // namespace sf

#endif // NEVERMORE_LEVELDB_VARINT_H