#include <mine_cart_madness.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>

Field parseInput(std::string_view input)
{
    Field ret;
    ret.track_height = 0;
    std::stringstream sstr{std::string(input)};
    std::string line;
    bool first_iter = true;
    int cart_id = 0;
    while(std::getline(sstr, line)) {
        if(first_iter) {
            assert(line.size() < std::numeric_limits<int>::max());
            ret.track_width = static_cast<int>(line.size());
            first_iter = false;
        }
        assert(line.size() == ret.track_width);
        for(int i=0; i<ret.track_width; ++i) {
            char const c = line[i];
            if(c == '<') {
                ret.carts.emplace_back(Cart{ std::make_tuple(i, ret.track_height), Cart::Left, 0, ++cart_id});
                ret.tracks.push_back('-');
            } else if(c == '>') {
                ret.carts.emplace_back(Cart{ std::make_tuple(i, ret.track_height), Cart::Right, 0, ++cart_id});
                ret.tracks.push_back('-');
            } else if(c == '^') {
                ret.carts.emplace_back(Cart{ std::make_tuple(i, ret.track_height), Cart::Up, 0, ++cart_id});
                ret.tracks.push_back('|');
            } else if(c == 'v') {
                ret.carts.emplace_back(Cart{ std::make_tuple(i, ret.track_height), Cart::Down, 0, ++cart_id});
                ret.tracks.push_back('|');
            } else {
                assert((c == ' ') || (c == '-') || (c == '|') || (c == '+') || (c == '/') || (c == '\\'));
                ret.tracks.push_back(line[i]);
            }
        }
        ++ret.track_height;
    }

    return ret;
}

std::vector<Cart> simulateStep(Field& f)
{
    std::sort(begin(f.carts), end(f.carts), [](Cart const& lhs, Cart const& rhs) {
        auto reverse = [](auto t) { return std::make_tuple(std::get<1>(t), std::get<0>(t)); };
        return reverse(lhs.coords) < reverse(rhs.coords);
    });
    std::vector<int> collided_carts;
    auto const has_collided = [&collided_carts](Cart const& c) {
        return std::find(begin(collided_carts), end(collided_carts), c.cart_id) != end(collided_carts);
    };
    for(auto& c : f.carts) {
        if(has_collided(c)) {
            // already collided carts no longer move
            continue;
        }
        auto const [x,y] = c.coords;
        assert(f.tracks[y * f.track_width + x] != ' ');
        if(c.direction == Cart::Left) {
            assert(x > 0);
            --std::get<0>(c.coords);
            char const new_track = f.tracks[y * f.track_width + (x - 1)];
            if(new_track == '/') {
                c.direction = Cart::Down;
            } else if(new_track == '\\') {
                c.direction = Cart::Up;
            } else if(new_track == '+') {
                if(c.turn_counter == 0) {
                    c.direction = Cart::Down;
                    c.turn_counter = 1;
                } else if(c.turn_counter == 1) {
                    // go straight
                    c.turn_counter = 2;
                } else {
                    assert(c.turn_counter == 2);
                    c.direction = Cart::Up;
                    c.turn_counter = 0;
                }
            } else {
                assert(new_track == '-');
            }
        } else if(c.direction == Cart::Right) {
            assert(x < f.track_width - 1);
            ++std::get<0>(c.coords);
            char const new_track = f.tracks[y * f.track_width + (x + 1)];
            if(new_track == '/') {
                c.direction = Cart::Up;
            } else if(new_track == '\\') {
                c.direction = Cart::Down;
            } else if(new_track == '+') {
                if(c.turn_counter == 0) {
                    c.direction = Cart::Up;
                    c.turn_counter = 1;
                } else if(c.turn_counter == 1) {
                    // go straight
                    c.turn_counter = 2;
                } else {
                    assert(c.turn_counter == 2);
                    c.direction = Cart::Down;
                    c.turn_counter = 0;
                }
            } else {
                assert(new_track == '-');
            }
        } else if(c.direction == Cart::Up) {
            assert(y > 0);
            --std::get<1>(c.coords);
            char const new_track = f.tracks[(y - 1) * f.track_width + x];
            if(new_track == '/') {
                c.direction = Cart::Right;
            } else if(new_track == '\\') {
                c.direction = Cart::Left;
            } else if(new_track == '+') {
                if(c.turn_counter == 0) {
                    c.direction = Cart::Left;
                    c.turn_counter = 1;
                } else if(c.turn_counter == 1) {
                    // go straight
                    c.turn_counter = 2;
                } else {
                    assert(c.turn_counter == 2);
                    c.direction = Cart::Right;
                    c.turn_counter = 0;
                }
            } else {
                assert(new_track == '|');
            }
        } else {
            assert(c.direction == Cart::Down);
            assert(y < f.track_height - 1);
            ++std::get<1>(c.coords);
            char const new_track = f.tracks[(y + 1) * f.track_width + x];
            if(new_track == '/') {
                c.direction = Cart::Left;
            } else if(new_track == '\\') {
                c.direction = Cart::Right;
            } else if(new_track == '+') {
                if(c.turn_counter == 0) {
                    c.direction = Cart::Right;
                    c.turn_counter = 1;
                } else if(c.turn_counter == 1) {
                    // go straight
                    c.turn_counter = 2;
                } else {
                    assert(c.turn_counter == 2);
                    c.direction = Cart::Left;
                    c.turn_counter = 0;
                }
            } else {
                assert(new_track == '|');
            }
        }

        // collision detection
        auto collision_it = std::find_if(begin(f.carts), end(f.carts), [c, has_collided](Cart const& c1) {
            return (c.coords == c1.coords) && (c.cart_id != c1.cart_id) && !has_collided(c1);
        });
        if(collision_it != end(f.carts)) {
            collided_carts.push_back(c.cart_id);
            collided_carts.push_back(collision_it->cart_id);
        }
    }

    // move all collided carts to return vector and remove them from field
    std::vector<Cart> ret;
    std::copy_if(begin(f.carts), end(f.carts), std::back_inserter(ret), has_collided);
    f.carts.erase(std::remove_if(begin(f.carts), end(f.carts), has_collided), end(f.carts));

    return ret;
}

Cart runToCompletion(Field& f)
{
    while(f.carts.size() > 1) { simulateStep(f); }
    return f.carts.front();
}
