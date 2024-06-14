#include "catch2/catch_test_macros.hpp"

#include "nevermore/Coding.h"
#include <cstdint>
#include <string>
#include <vector>

TEST_CASE("test fixed_int coding", "[fixed_int]") {

    SECTION("test fixed32") {
        std::string s{};
        for (std::uint32_t v = 0; v < 100000; ++v)
            sf::put_fixed32(s, v);

        const auto* p = s.data();
        for (auto v = 0; v < 100000; ++v) {
            auto actual = sf::decode_fixed32(p);
            REQUIRE(actual == v);
            p += sizeof(std::uint32_t);
        }
    }

    SECTION("test fixed64") {
        std::string s{};
        for (int power = 0; power <= 63; ++power) {
            std::uint64_t v = 1 << power;
            sf::put_fixed64(s, v - 1);
            sf::put_fixed64(s, v + 0);
            sf::put_fixed64(s, v + 1);
        }

        const auto* p = s.data();
        for (int power = 0; power <= 63; ++power) {
            std::uint64_t v = 1 << power;
            auto actual = sf::decode_fixed64(p);
            REQUIRE(v - 1 == actual);
            p += sizeof(uint64_t);

            actual = sf::decode_fixed64(p);
            REQUIRE(v + 0 == actual);
            p += sizeof(uint64_t);

            actual = sf::decode_fixed64(p);
            REQUIRE(v + 1 == actual);
            p += sizeof(uint64_t);
        } 
    }

    SECTION("Test that encoding routines generate little-endian encodings") {
        std::string dst;
        sf::put_fixed32(dst, 0x04030201);
        REQUIRE(dst.size() == 4);
        REQUIRE(static_cast<int>(dst[0]) == 0x01);
        REQUIRE(static_cast<int>(dst[1]) == 0x02);
        REQUIRE(static_cast<int>(dst[2]) == 0x03);
        REQUIRE(static_cast<int>(dst[3]) == 0x04);

        dst.clear();
        sf::put_fixed64(dst, 0x0807060504030201);
        REQUIRE(static_cast<int>(dst[0]) == 0x01);
        REQUIRE(static_cast<int>(dst[1]) == 0x02);
        REQUIRE(static_cast<int>(dst[2]) == 0x03);
        REQUIRE(static_cast<int>(dst[3]) == 0x04);
        REQUIRE(static_cast<int>(dst[4]) == 0x05);
        REQUIRE(static_cast<int>(dst[5]) == 0x06);
        REQUIRE(static_cast<int>(dst[6]) == 0x07);
        REQUIRE(static_cast<int>(dst[7]) == 0x08);
    }

}

TEST_CASE("test varint encoding", "[varint]") {

    SECTION("test varint32") {
        constexpr auto test_size = 32 * 32;
        std::string s{};
        for (std::uint32_t i = 0; i < test_size; ++i) {
            std::uint32_t v = (i / 32) << (i % 32);
            sf::put_varint32(s, v);
        }

        const auto* p = s.data();
        const auto* limit = p + s.size();

        for (std::uint32_t i = 0; i < test_size; ++i) {
            std::uint32_t expected = (i / 32) << (i % 32);
            std::uint32_t actual;
            const auto* start = p;
            p = sf::get_varint32_ptr(p, limit, actual);
            REQUIRE(p != nullptr);
            REQUIRE(actual == expected);
            REQUIRE(sf::varint_length(actual) == (p - start));
        }

        REQUIRE(p == (s.data() + s.size())); 
    }

    SECTION("test varint64") {
        std::vector<std::uint64_t> values;
        values.push_back(0);
        values.push_back(100);
        values.push_back(~0);
        values.push_back(~0 - 1);
        
        for (auto k = 0; k < 64; ++k) {
            const std::uint64_t power = 1ull << k;
            values.push_back(power);
            values.push_back(power - 1);
            values.push_back(power + 1);
        }
        
        std::string s{};
        for (std::size_t i = 0; i < values.size(); ++i)
            sf::put_varint64(s, values[i]);
        
        const char* p = s.data();
        const char* limit = p + s.size();
        for (std::size_t i = 0; i < values.size(); ++i) {
            REQUIRE(p < limit);
            std::uint64_t actual;
            const char* start = p;
            p = sf::get_varint64_ptr(p, limit, actual);

            REQUIRE(p != nullptr);
            REQUIRE(values[i] == actual);
            REQUIRE(sf::varint_length(actual) == (p - start));
        }

        REQUIRE(p == limit);
    }

    SECTION("test varint32 overflow") {
        
    }

}