#include <catch.hpp>

#include <lib.hpp>

TEST_CASE("Lib")
{
    SECTION("Foo")
    {
        REQUIRE(foo(0) == 5);
        CHECK(foo(-5) == 0);
        CHECK(foo(37) == 42);
    }
}
