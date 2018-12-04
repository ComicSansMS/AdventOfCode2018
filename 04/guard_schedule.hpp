#ifndef ADVENT_OF_CODE_04_GUARD_SCHEDULE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_GUARD_SCHEDULE_HPP_INCLUDE_GUARD

#include <date/date.h>

#include <chrono>
#include <ostream>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

struct ScheduleEntry {
    std::string event;
    date::year_month_day date;
    date::local_time<std::chrono::minutes> timestamp;
};

std::vector<ScheduleEntry> parseInput(std::string_view input);

struct SleepTime {
    date::year_month_day date;
    date::local_time<std::chrono::minutes> start_sleep;
    std::chrono::minutes sleep_duration;
};

std::unordered_map<int, std::vector<SleepTime>> calculateSleepSchedule(std::vector<ScheduleEntry> const& schedule);

std::chrono::minutes totalSleepTimeForGuard(std::vector<SleepTime> const& sleep_times);

int findGuardWithMostSleep(std::unordered_map<int, std::vector<SleepTime>> const& sleep_schedule);

std::tuple<int, int> findSleepiestMinute(std::vector<SleepTime> const& sleep_times);

int calculateStrategy1(std::unordered_map<int, std::vector<SleepTime>> const& sleep_schedule);

int calculateStrategy2(std::unordered_map<int, std::vector<SleepTime>> const& sleep_schedule);

#endif
