#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Test01") {
    REQUIRE (1 + 1 == 2);
    REQUIRE (1 + 1 != 3);
}

TEST_CASE("Test02 Fail") {
    REQUIRE (420 * 69 == 96);
}