#include "catch2/catch_test_macros.hpp"

#include "nevermore/posix/Memory.h"

TEST_CASE("test mmap_unique", "[mmap_unique]") 
{
    SECTION("mmap_unique")
    {
        auto ptr = sf::mmap_unique<int>(42);
        REQUIRE(*ptr == 42);
    }
}

TEST_CASE("test mmap_unique_server", "[mmap_unique_server]")
{
    SECTION("client can get the same value from server")
    {
        auto ptr1 = sf::mmap_unique_server<int>(100);
        auto ptr2 = sf::mmap_unique_client<int>();

        REQUIRE(*ptr1 == *ptr2);
    }
}
