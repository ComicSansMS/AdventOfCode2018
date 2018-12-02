#include <warehouse_ids.hpp>

#include <range/v3/core.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
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
    auto const idDiffAndKeepFirstString = []() {
        return ranges::view::transform([](auto ids) -> std::tuple<std::optional<int>, std::string_view> {
            return { stringDifferenceIsOne(std::get<0>(ids), std::get<1>(ids)), std::get<0>(ids) };
        });
    };
    auto const filterDiffIsOne = []() {
        return ranges::view::filter([](auto diff_id_pair) -> bool { return std::get<0>(diff_id_pair).has_value(); });
    };
    auto rng = ranges::view::cartesian_product(ids, ids) | idDiffAndKeepFirstString() | filterDiffIsOne();
    if(rng.empty()) { return ""; }
    auto const& [diff, id_str] = *rng.begin();
    std::string ret{ id_str };
    ret.erase(*diff, 1);
    return ret;
}
