#include <warehouse_ids.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Warehouse Ids")
{
    char const* sample_input = "abcdef\nbababc\nabbcde\nabcccd\naabcdd\nabcdee\nababab\n";

    SECTION("Parse Input")
    {
        auto ids = parseInput(sample_input);
        REQUIRE(ids.size() == 7);
        CHECK(ids[0] == "abcdef");
        CHECK(ids[1] == "bababc");
        CHECK(ids[2] == "abbcde");
        CHECK(ids[3] == "abcccd");
        CHECK(ids[4] == "aabcdd");
        CHECK(ids[5] == "abcdee");
        CHECK(ids[6] == "ababab");
    }

    SECTION("Occurence Equality")
    {
        CHECK(Occurences{true, true} == Occurences{true, true});
        CHECK(Occurences{true, false} == Occurences{true, false});
        CHECK(Occurences{false, true} == Occurences{false, true});
        CHECK(Occurences{false, false} == Occurences{false, false});
        CHECK_FALSE(Occurences{true, true} == Occurences{true, false});
    }

    SECTION("Occurence Ostream Inserter")
    {
        std::stringstream sstr;
        sstr << Occurences{true, false};
        CHECK(sstr.str() == "{2: true, 3: false}");
    }

    SECTION("Count Letters")
    {
        CHECK(countLetters("abcdef") == Occurences{false, false});
        CHECK(countLetters("bababc") == Occurences{true, true});
        CHECK(countLetters("abbcde") == Occurences{true, false});
        CHECK(countLetters("abcccd") == Occurences{false, true});
        CHECK(countLetters("aabcdd") == Occurences{true, false});
        CHECK(countLetters("abcdee") == Occurences{true, false});
        CHECK(countLetters("ababab") == Occurences{false, true});
    }

    SECTION("Calculate checksum")
    {
        auto ids = parseInput(sample_input);
        CHECK(calculateChecksum(ids) == 12);
    }

    SECTION("String difference")
    {
        CHECK(!stringDifferenceIsOne("abcde", "axcye"));
        REQUIRE(stringDifferenceIsOne("fghij", "fguij"));
        CHECK(*stringDifferenceIsOne("fghij", "fguij") == 2);
        CHECK(!stringDifferenceIsOne("abcde", "abcde"));
    }

    SECTION("Common code")
    {
        std::vector<std::string> ids{ "abcde", "fghij", "klmno", "pqrst", "fguij", "axcye", "wvxyz" };
        CHECK(commonCode(ids) == "fgij");
        CHECK(commonCode({"abc", "abc"}) == "");
    }
}
