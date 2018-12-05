#ifndef ADVENT_OF_CODE_05_POLYMER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_05_POLYMER_HPP_INCLUDE_GUARD

#include <list>
#include <string>
#include <string_view>

using Polymer = std::list<char>;

Polymer fromString(std::string_view input);

std::string toString(Polymer const& p);

Polymer::iterator reduce(Polymer& l, Polymer::iterator start);

void reduceAll(Polymer& p);

Polymer removeAndReduce(Polymer const& p);

#endif
