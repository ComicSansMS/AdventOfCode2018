#include <polymer.hpp>

#include <catch.hpp>

TEST_CASE("Polymer")
{
    SECTION("Polymer string conversion")
    {
        CHECK(fromString("aAbBcCdD") == Polymer{'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D'});
        CHECK(toString(Polymer{'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D'}) == "aAbBcCdD");
        CHECK(toString(fromString("fooberella")) == "fooberella");
    }

    SECTION("Reduce")
    {
        Polymer p = fromString("Aa");
        auto it = reduce(p, begin(p));
        CHECK(toString(p) == "");
        CHECK(it == end(p));

        p = fromString("aA");
        it = reduce(p, begin(p));
        CHECK(toString(p) == "");
        CHECK(it == end(p));

        p = fromString("abBA");
        it = reduce(p, begin(p));
        CHECK(toString(p) == "aA");
        CHECK(it == begin(p));

        p = fromString("abAB");
        it = reduce(p, begin(p));
        CHECK(toString(p) == "abAB");
        CHECK(it == end(p));

        p = fromString("aabAAB");
        it = reduce(p, begin(p));
        CHECK(toString(p) == "aabAAB");
        CHECK(it == end(p));

        p = fromString("aAbB");
        it = begin(p);
        std::advance(it, 2);
        it = reduce(p, it);
        CHECK(toString(p) == "aA");
        CHECK(it == ++begin(p));
    }

    SECTION("Reduce All")
    {
        Polymer p = fromString("dabAcCaCBAcCcaDA");
        reduceAll(p);
        CHECK(toString(p) == "dabCBAcaDA");
    }

    SECTION("Remove and Reduce")
    {
        Polymer p = fromString("dabAcCaCBAcCcaDA");
        CHECK(toString(removeAndReduce(p)) == "daDA");
    }
}
