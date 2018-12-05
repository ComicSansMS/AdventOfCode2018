#include <polymer.hpp>

#include <range/v3/core.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>

Polymer fromString(std::string_view input)
{
    Polymer ret;
    std::copy(begin(input), end(input), std::back_inserter(ret));
    return ret;
}

std::string toString(Polymer const& p)
{
    std::string ret;
    ret.reserve(p.size());
    std::copy(begin(p), end(p), std::back_inserter(ret));
    return ret;
}


Polymer::iterator reduce(Polymer& l, Polymer::iterator start)
{
    auto it = std::adjacent_find(start, end(l), [](char c1, char c2) {
        if(c2 < c1) { std::swap(c1, c2); }
        return (c2 - c1) == ('a' - 'A');
    });
    if(it != end(l)) {
        it = l.erase(l.erase(it));
        if(it != begin(l)) { --it; }
    }
    return it;
}

void reduceAll(Polymer& p)
{
    auto const old_size = p.size();
    auto it = begin(p);
    while(it != end(p)) {
        it = reduce(p, it);
    }
}

Polymer removeAndReduce(Polymer const& p)
{
    Polymer ret;
    auto minsize = p.size();
    for(char i = 'a'; i <= 'z'; ++i) {
        Polymer pi = p;
        pi.remove_if([i](char c) { return (c == i) || (c == i - ('a' - 'A')); });
        reduceAll(pi);
        if(pi.size() < minsize) {
            minsize = pi.size();
            ret = std::move(pi);
        }
    }
    return ret;
}
