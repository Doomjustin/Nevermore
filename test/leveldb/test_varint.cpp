#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <sstream>

#include "nevermore/Varint.h"

TEST_CASE("test varint<uint32_t>", "[varint]") 
{
    SECTION("decode integral") 
    {
        sf::Varint var{ 123 };

        REQUIRE(var.value() == 123);
    }

    SECTION("decode str")
    {
        auto result = 0b1110101;

        // 没有显示指定的情况下，默认为std::uint32_t
        sf::Varint var{ "01110101" };

        REQUIRE(var.value() == result);
    }

    SECTION("decode str uint16")
    {
        auto result = 0b1111111110101;
        sf::Varint var{ "0011111111110101" };

        REQUIRE(var.value() == result);
    }

    SECTION("encode uint8")
    {
        sf::Varint var{ 0b1110101 };
        char buf[1];
        buf[0] = static_cast<char>(0b01110101);
        std::stringstream ss{};
        var.encode(ss);

        REQUIRE_THAT(ss.str(), Catch::Matchers::Equals(buf));
    }

    SECTION("encode uint16")
    {
        sf::Varint var{ 0b1111111110101 };
        char buf[2];
        buf[0] = static_cast<char>(0b11110101);
        buf[1] = static_cast<char>(0b00111111);
        std::stringstream ss{};
        var.encode(ss);

        REQUIRE_THAT(ss.str(), Catch::Matchers::Equals(buf));
    }

    SECTION("encode uint32")
    {
        sf::Varint var{ 0b11011111110111111101111111110101 };
        char buf[5];
        buf[0] = static_cast<char>(0b11110101);
        buf[1] = static_cast<char>(0b10111111);
        buf[2] = static_cast<char>(0b11111111);
        buf[3] = static_cast<char>(0b11111110);
        buf[4] = static_cast<char>(0b00001101);
        std::stringstream ss{};
        var.encode(ss);
        
        REQUIRE_THAT(ss.str(), Catch::Matchers::Equals(buf));
    }
}