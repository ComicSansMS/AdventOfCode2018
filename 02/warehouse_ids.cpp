#include <warehouse_ids.hpp>

#include <range/v3/core.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/zip.hpp>

#include <array>
#include <cassert>
#include <string>

std::vector<std::string> parseInput(std::string_view input)
{
    std::vector<std::string> ret;

    for(auto const& str : (input | ranges::view::split('\n'))) {
        ret.emplace_back(str);
    }
    return ret;
}

Occurences countLetters(std::string_view id)
{
    std::array<int, 26> counts = {};
    for(auto c : id) {
        assert((c >= 'a') && (c <= 'z'));
        ++counts[c - 'a'];
    }
    Occurences ret = {false, false};
    for(int c : counts) {
        if(c == 2) { ret.occursTwice = true; }
        else if(c == 3) { ret.occursThrice = true; }
    }
    return ret;
}

int calculateChecksum(std::vector<std::string> const& ids)
{
    int count_twices = 0;
    int count_thrices = 0;
    for(auto const& id : ids) {
        auto const [occursTwice, occursThrice] = countLetters(id);
        if(occursTwice) { ++count_twices; }
        if(occursThrice) { ++count_thrices; }
    }
    return count_twices * count_thrices;
}

std::optional<int> stringDifferenceIsOne(std::string_view str1, std::string_view str2)
{
    assert(str1.length() == str2.length());

    int differenceAt = -1;
    int i = 0;
    auto rng = ranges::view::zip(str1, str2) |
        ranges::view::filter([&](auto z) {
            if(std::get<0>(z) == std::get<1>(z)) { ++i; return false; }
            else { differenceAt = i; return true; }
        }) |
        ranges::view::take(2);
    return (ranges::distance(rng) == 1) ? std::optional<int>(differenceAt) : std::nullopt;
}

std::string commonCode(std::vector<std::string> const& ids)
{
    int const ids_size = static_cast<int>(ids.size());
    for(int i = 0; i < ids_size; ++i) {
        for(int j = i; j < ids_size; ++j) {
            auto diff_one = stringDifferenceIsOne(ids[i], ids[j]);
            if(diff_one) {
                std::string ret = ids[i];
                ret.erase(*diff_one, 1);
                return ret;
            }
        }
    }
    return "";
}
