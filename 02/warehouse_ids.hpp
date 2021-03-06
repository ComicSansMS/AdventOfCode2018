#ifndef ADVENT_OF_CODE_02_WAREHOUSE_IDS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_WAREHOUSE_IDS_HPP_INCLUDE_GUARD

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> parseInput(std::string_view input);

struct Occurences {
    bool occursTwice;
    bool occursThrice;
};
inline bool operator==(Occurences const& lhs, Occurences const& rhs) {
    return (lhs.occursTwice == rhs.occursTwice) && (lhs.occursThrice == rhs.occursThrice);
}
inline std::ostream& operator<<(std::ostream& os, Occurences const& oc) {
    return os << "{2: " << std::boolalpha << oc.occursTwice << ", 3: " << std::boolalpha << oc.occursThrice << "}";
}
Occurences countLetters(std::string_view id);

int calculateChecksum(std::vector<std::string> const& ids);

std::optional<int> stringDifferenceIsOne(std::string_view str1, std::string_view str2);

std::string commonCode(std::vector<std::string> const& ids);

#endif
