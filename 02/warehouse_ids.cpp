#include <warehouse_ids.hpp>

#include <array>
#include <cassert>
#include <iterator>
#include <sstream>
#include <string>

std::vector<std::string> parseInput(std::string_view input)
{
    std::vector<std::string> ret;

    std::stringstream sstr{std::string(input)};
    for (std::string line; std::getline(sstr, line);) {
        ret.emplace_back(std::move(line));
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
    auto it1 = str1.begin();
    auto const it1_end = str1.end();
    auto it2 = str2.begin();
    int firstDifferenceEncounteredAt = -1;
    for(int i=0; i<static_cast<int>(str1.length()); ++i) {
        if(str1[i] != str2[i]) {
            if(firstDifferenceEncounteredAt != -1) { 
                return std::nullopt;
            } else {
                firstDifferenceEncounteredAt = i;
            }
        }
    }
    return (firstDifferenceEncounteredAt != -1) ? std::optional<int>(firstDifferenceEncounteredAt) : std::nullopt;
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
