#ifndef ADVENT_OF_CODE_04_GUARD_SCHEDULE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_GUARD_SCHEDULE_HPP_INCLUDE_GUARD

#include <date/date.h>

#include <chrono>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

struct ScheduleEntry {
    std::string event;
    date::local_time<std::chrono::minutes> timestamp;
};

std::vector<ScheduleEntry> parseInput(std::string_view input);

struct SleepTime {
    date::local_time<std::chrono::minutes> start_sleep;
    std::chrono::minutes sleep_duration;
};

std::unordered_map<int, std::vector<SleepTime>> calculateSleepTimes(std::vector<ScheduleEntry> const& schedule);

#endif
