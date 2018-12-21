#include <regex_labyrinth.hpp>

#include <algorithm>
#include <cassert>
#include <deque>
#include <iterator>
#include <limits>
#include <ostream>
#include <regex>

bool operator==(Vec2 const& lhs, Vec2 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

Room::Room(Vec2 const& pos)
    :position(pos), adjacencies{false, false, false, false}
{}

Map constructMap(std::string_view regex)
{
    Map ret;
    assert((regex.front() == '^') && (regex.back() == '$'));

    Vec2 current_position{0, 0};
    ret.min = Vec2{0, 0};
    ret.max = Vec2{0, 0};
    ret.rooms[Vec2{0, 0}] = Room(Vec2{0, 0});
    Room* current_room = &(ret.rooms.begin()->second);
    std::vector<Vec2> stack;
    for(std::size_t i = 1; i < regex.length() - 1; ++i) {
        char const c = regex[i];
        if(c == '(') {
            stack.push_back(current_position);
        } else if(c == ')') {
            stack.pop_back();
        } else if(c == '|') {
            current_position = stack.back();
            current_room = &ret.rooms[current_position];
        } else if(c == 'N') {
            --current_position.y;
            ret.min.y = std::min(ret.min.y, current_position.y);
            current_room->adjacencies[Room::North] = true;
            if(ret.rooms.find(current_position) == end(ret.rooms)) {
                current_room = &(ret.rooms.emplace(std::make_pair(current_position, Room(current_position))).first->second);
            } else {
                current_room = &(ret.rooms[current_position]);
            }
            current_room->adjacencies[Room::South] = true;
        } else if(c == 'S') {
            ++current_position.y;
            ret.max.y = std::max(ret.max.y, current_position.y);
            current_room->adjacencies[Room::South] = true;
            if(ret.rooms.find(current_position) == end(ret.rooms)) {
                current_room = &(ret.rooms.emplace(std::make_pair(current_position, Room(current_position))).first->second);
            } else {
                current_room = &(ret.rooms[current_position]);
            }
            current_room->adjacencies[Room::North] = true;
        } else if(c == 'W') {
            --current_position.x;
            ret.min.x = std::min(ret.min.x, current_position.x);
            current_room->adjacencies[Room::West] = true;
            if(ret.rooms.find(current_position) == end(ret.rooms)) {
                current_room = &(ret.rooms.emplace(std::make_pair(current_position, Room(current_position))).first->second);
            } else {
                current_room = &(ret.rooms[current_position]);
            }
            current_room->adjacencies[Room::East] = true;
        } else {
            assert(c == 'E');
            ++current_position.x;
            ret.max.x = std::max(ret.max.x, current_position.x);
            current_room->adjacencies[Room::East] = true;
            if(ret.rooms.find(current_position) == end(ret.rooms)) {
                current_room = &(ret.rooms.emplace(std::make_pair(current_position, Room(current_position))).first->second);
            } else {
                current_room = &(ret.rooms[current_position]);
            }
            current_room->adjacencies[Room::West] = true;
        }
    }
    return ret;
}

std::ostream& operator<<(std::ostream& os, Map const& m)
{
    bool is_top_row = true;
    for(int y = m.min.y; y <= m.max.y; ++y) {
        if(is_top_row) {
            os << '#';
            for(int x=m.min.x; x <= m.max.x; ++x) {
                assert(!m.rooms.find(Vec2(x, y))->second.adjacencies[Room::North]);
                os << '#' << '#';
            }
            is_top_row = false;
            os << '\n';
        }
        // middle_row
        bool neighbour_had_door = false;
        for(int x=m.min.x; x <= m.max.x; ++x) {
            if(x == m.min.x) {
                assert(!m.rooms.find(Vec2(x, y))->second.adjacencies[Room::West]);
                os << '#';
            }
            os << ((x == 0 && y == 0) ? 'X' : '.');
            assert(neighbour_had_door == (m.rooms.find(Vec2(x, y))->second.adjacencies[Room::West]));
            neighbour_had_door = (m.rooms.find(Vec2(x, y))->second.adjacencies[Room::East]);
            os << (neighbour_had_door ? '|' : '#');
        }
        os << '\n';
        // bottom row
        for(int x=m.min.x; x <= m.max.x; ++x) {
            if(x == m.min.x) {
                assert(!m.rooms.find(Vec2(x, y))->second.adjacencies[Room::West]);
                os << '#';
            }
            os << ((m.rooms.find(Vec2(x, y))->second.adjacencies[Room::South]) ? '-' : '#');
            os << '#';
        }
        os << '\n';
    }
    return os;
}

std::tuple<int, int> shortestPaths(Map const& m)
{
    std::deque<Vec2> stack;
    std::unordered_map<Vec2, int> distances;
    int current_distance = 0;
    int long_paths = 0;

    auto push_neighbor = [&stack, &distances, &current_distance](int position_x, int position_y) {
        Vec2 const position(position_x, position_y);
        auto it_n = distances.find(position);
        if(it_n == end(distances)) {
            distances[position] = current_distance + 1;
            stack.push_back(position);
        } else {
            assert(it_n->second < current_distance);
        }
    };

    stack.push_back(Vec2(0, 0));
    distances[Vec2(0, 0)] = current_distance;

    while(!stack.empty()) {
        Vec2 const pos = stack.front();
        stack.pop_front();
        assert((distances.find(pos) != end(distances)) && ((distances[pos] == current_distance) || (distances[pos] == current_distance + 1)));
        current_distance = distances[pos];
        if(current_distance >= 1000) { ++long_paths; }
        Room const& r = m.rooms.find(pos)->second;
        if(r.adjacencies[Room::North]) { push_neighbor(r.position.x, r.position.y - 1); }
        if(r.adjacencies[Room::South]) { push_neighbor(r.position.x, r.position.y + 1); }
        if(r.adjacencies[Room::West])  { push_neighbor(r.position.x - 1, r.position.y); }
        if(r.adjacencies[Room::East])  { push_neighbor(r.position.x + 1, r.position.y); }
    }
    return std::make_tuple(current_distance, long_paths);
}
