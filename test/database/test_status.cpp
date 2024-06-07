#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

#include "nevermore/Status.h"

TEST_CASE( "test status move constructor", "[Status]" ) {
    using namespace sf;

    SECTION("Status Ok") {
        auto ok = Status::ok();
        auto ok2 = std::move(ok);

        REQUIRE(ok2.is_ok());
    }

    SECTION("Status NotFound") {
        auto status = Status::not_found("custom NotFound status message");
        auto status2 = std::move(status);

        REQUIRE(status2.is_not_found());
        REQUIRE_THAT(status2.to_string(), Catch::Matchers::Equals("NotFound: custom NotFound status message"));
    }

    SECTION("self moved") {
        auto self_moved = Status::io_error("custom IOError status message");
        // Needed to bypass compiler warning about explicit move-assignment.
        auto& self_moved_reference = self_moved;
        
        REQUIRE_NOTHROW(self_moved_reference = std::move(self_moved));
    }
}
