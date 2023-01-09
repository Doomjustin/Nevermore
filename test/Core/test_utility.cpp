#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <Nevermore/Utility.h>

#include <numeric>
#include <vector>
#include <future>


constexpr static int TEST_SIZE = 1000000;

std::vector<int>& create_natural_numbers(int size = TEST_SIZE)
{
    static auto numbers = std::vector<int>(size);  
    std::iota(numbers.begin(), numbers.end(), 0); 

    return numbers;
}

TEST_CASE("is_even(t)", "[number]")
{
    auto numbers = create_natural_numbers();

    SECTION("is odd number", "[Functional]")
    {
        CHECK(SF::is_odd(99));
    }

    SECTION("not odd number", "[Functional]")
    {
        CHECK_FALSE(SF::is_odd(100));
    }

    SECTION("odd number", "[Performance]")
    {
        for (auto e: numbers)
        SF::is_odd(e);    
    }
}

TEST_CASE("is_odd(t)", "[number]")
{
    auto numbers = create_natural_numbers();

    SECTION("is odd number", "[Functional]")
    {
        CHECK(SF::is_odd(99));
    }

    SECTION("not odd number", "[Functional]")
    {
        CHECK_FALSE(SF::is_odd(100));
    }

    SECTION("odd number", "[Performance]")
    {
        for (auto e: numbers)
        SF::is_odd(e);    
    }
}

TEST_CASE("case exchange", "[string]")
{
    std::string lower_case_string = "abcdefghijklmnopqrstuvwxyz";
    std::string upper_case_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    SECTION("to_upper", "[Functional]")
    {
        auto res = SF::to_upper(lower_case_string);

        REQUIRE_THAT(res, Catch::Matchers::Equals(upper_case_string));
    }

    SECTION("to_lower", "[Functional]")
    {
        auto res = SF::to_lower(lower_case_string);

        REQUIRE_THAT(res, Catch::Matchers::Equals(lower_case_string));
    }
}

TEST_CASE("trim family", "[string]")
{
    std::string base_str = " 1 2   34 5     6  ";
    std::string trimed_str = "123456";
    std::string left_trimed_str = "1 2   34 5     6  ";
    std::string right_trimed_str = " 1 2   34 5     6";

    SECTION("trim", "[Functional]")
    {
        auto res = SF::trim(base_str);

        REQUIRE_THAT(res, Catch::Matchers::Equals(trimed_str));
    }

    SECTION("left trim", "[Functional]")
    {
        auto res = SF::left_trim(base_str);

        REQUIRE_THAT(res, Catch::Matchers::Equals(left_trimed_str));
    }

    SECTION("right trim", "[Functional]")
    {
        auto res = SF::right_trim(base_str);

        REQUIRE_THAT(res, Catch::Matchers::Equals(right_trimed_str));
    }

}

struct Point {
    Point(int x, int y)
        : X{ x }, Y{ y }
    {}

    // Point(const Point&) = delete;
    // Point& operator=(const Point&) = delete;
    SF_DISABLE_COPY(Point)

    // Point(Point&&) noexcept = default;
    // Point& operator=(Point&&) noexcept = default;
    SF_DEFAULT_MOVE(Point)

    ~Point() {}

    int X;
    int Y;
};

TEST_CASE("make_singleton<T>(args...)", "[singleton]")
{
  
    SECTION("single thread", "[Functional]")
    {
        auto& p1 = SF::make_singleton<Point>(1, 2);
        auto& p2 = SF::make_singleton<Point>(3, 4);

        REQUIRE(p1.X == p2.X);
        REQUIRE(p1.Y == p2.Y);
        REQUIRE(&p1 == &p2);
    }

    SECTION("multiply threads", "[Functional]")
    {
        constexpr static int TASKS = 100000;

        std::vector<std::future<Point&>> results{};

        for (int i = 0; i != TASKS; ++i) {
            auto res = std::async(SF::make_singleton<Point, int, int>, i, i + 1);
            results.push_back(std::move(res));
        }
        
        auto& base_point = results[0].get();
        // every Point should be a same one
        for (int i = 1; i < TASKS; ++i) {
            if (!results[1].valid()) continue;

            Point& tmp = results[1].get();
            REQUIRE(base_point.X == tmp.X);
            REQUIRE(base_point.Y == tmp.Y);
            REQUIRE(&base_point == &tmp);
        }
    }

}