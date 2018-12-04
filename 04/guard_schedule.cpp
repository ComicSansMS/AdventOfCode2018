#include <guard_schedule.hpp>

#include <range/v3/core.hpp>

#include <array>
#include <cassert>
#include <limits>
#include <regex>
#include <string>

namespace {

}

std::vector<ScheduleEntry> parseInput(std::string_view input)
{
    std::regex rx_line(R"x(\[(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d)\] (.+))x");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<ScheduleEntry> entries;
    auto const year = date::year(1518);
    for(auto it = it_begin; it != it_end; ++it) {
        std::match_results<std::string_view::iterator> match = *it;
        assert(std::string(match[1]) == "1518");
        auto const month = date::month(std::stoi(match[2]));
        auto const day = date::day(std::stoi(match[3]));
        auto const hour = std::chrono::hours(std::stoi(match[4]));
        auto const minute = std::chrono::minutes(std::stoi(match[5]));
        auto const date = date::year_month_day(year, month, day);
        auto const time = date::make_time(hour, minute, 0);
        auto const timestamp = date::local_time<date::days>(date) + time.to_duration();
        auto const evt = match[6];
        entries.push_back(ScheduleEntry{evt, timestamp});
    }

    std::sort(begin(entries), end(entries),
              [](ScheduleEntry const& lhs, ScheduleEntry const& rhs) { return lhs.timestamp < rhs.timestamp; });

    return entries;
}

std::unordered_map<int, std::vector<SleepTime>> calculateSleepTimes(std::vector<ScheduleEntry> const& schedule)
{
    std::unordered_map<int, std::vector<SleepTime>> sleep_times;
    int current_guard = -1;
    date::local_time<std::chrono::minutes> current_start_sleep = date::local_time<std::chrono::minutes>();
    for(auto const& e : schedule) {
        if(e.event.substr(0, 7) == "Guard #") {
            current_guard = std::stoi(std::string{e.event.substr(7)});
            sleep_times[current_guard];
        } else if(e.event == "falls asleep") {
            assert(current_guard != -1);
            sleep_times[current_guard].push_back(SleepTime{e.timestamp, std::chrono::minutes(0)});
        } else {
            assert(e.event == "wakes up");
            assert(current_guard != -1);
            std::vector<SleepTime>& st = sleep_times[current_guard];
            assert(!st.empty());
            assert(st.back().sleep_duration == std::chrono::minutes(0));
            assert(st.back().start_sleep < e.timestamp);
            auto const sleep_duration = e.timestamp - st.back().start_sleep;
            st.back().sleep_duration = sleep_duration;
        }
    }
    return sleep_times;
}
