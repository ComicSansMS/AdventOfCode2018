#include <guard_schedule.hpp>

#include <catch.hpp>

#include <sstream>
#include <string>

TEST_CASE("Guard Schedule")
{
    char const* sample_input = R"([1518-11-01 00:05] falls asleep
[1518-11-01 00:25] wakes up
[1518-11-01 00:30] falls asleep
[1518-11-01 00:55] wakes up
[1518-11-01 00:00] Guard #10 begins shift
[1518-11-02 00:40] falls asleep
[1518-11-02 00:50] wakes up
[1518-11-03 00:05] Guard #10 begins shift
[1518-11-03 00:24] falls asleep
[1518-11-03 00:29] wakes up
[1518-11-04 00:02] Guard #99 begins shift
[1518-11-01 23:58] Guard #99 begins shift
[1518-11-04 00:36] falls asleep
[1518-11-04 00:46] wakes up
[1518-11-05 00:03] Guard #99 begins shift
[1518-11-05 00:45] falls asleep
[1518-11-05 00:55] wakes up
)";

    SECTION("Parse Input")
    {
        using namespace date::literals;
        using namespace std::literals::chrono_literals;
        auto const schedule = parseInput(sample_input);
        REQUIRE(schedule.size() == 17);

        CHECK(schedule[0].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 0min, 0).to_duration());
        CHECK(schedule[0].event == "Guard #10 begins shift");

        CHECK(schedule[1].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 5min, 0).to_duration());
        CHECK(schedule[1].event == "falls asleep");

        CHECK(schedule[2].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 25min, 0).to_duration());
        CHECK(schedule[2].event == "wakes up");

        CHECK(schedule[3].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 30min, 0).to_duration());
        CHECK(schedule[3].event == "falls asleep");

        CHECK(schedule[4].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 55min, 0).to_duration());
        CHECK(schedule[4].event == "wakes up");

        CHECK(schedule[5].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(23h, 58min, 0).to_duration());
        CHECK(schedule[5].event == "Guard #99 begins shift");

        CHECK(schedule[6].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/2)) +
            date::make_time(0h, 40min, 0).to_duration());
        CHECK(schedule[6].event == "falls asleep");

        CHECK(schedule[7].timestamp ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/2)) +
            date::make_time(0h, 50min, 0).to_duration());
        CHECK(schedule[7].event == "wakes up");
    }

    SECTION("Calculate Sleep Times From Schedule")
    {
        using namespace date::literals;
        using namespace std::literals::chrono_literals;

        auto const schedule = parseInput(sample_input);
        auto sleep_schedule = calculateSleepSchedule(schedule);

        REQUIRE(sleep_schedule.size() == 2);
        REQUIRE(sleep_schedule.find(10) != end(sleep_schedule));
        REQUIRE(sleep_schedule.find(99) != end(sleep_schedule));

        REQUIRE(sleep_schedule[10].size() == 3);

        CHECK(sleep_schedule[10][0].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 5min, 0).to_duration());
        CHECK(sleep_schedule[10][0].sleep_duration == 20min);

        CHECK(sleep_schedule[10][1].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/1)) +
            date::make_time(0h, 30min, 0).to_duration());
        CHECK(sleep_schedule[10][1].sleep_duration == 25min);

        CHECK(sleep_schedule[10][2].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/3)) +
            date::make_time(0h, 24min, 0).to_duration());
        CHECK(sleep_schedule[10][2].sleep_duration == 5min);

        REQUIRE(sleep_schedule[99].size() == 3);

        CHECK(sleep_schedule[99][0].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/2)) +
            date::make_time(0h, 40min, 0).to_duration());
        CHECK(sleep_schedule[99][0].sleep_duration == 10min);

        CHECK(sleep_schedule[99][1].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/4)) +
            date::make_time(0h, 36min, 0).to_duration());
        CHECK(sleep_schedule[99][1].sleep_duration == 10min);

        CHECK(sleep_schedule[99][2].start_sleep ==
            date::local_time<std::chrono::minutes>(date::local_days(1518_y/11/5)) +
            date::make_time(0h, 45min, 0).to_duration());
        CHECK(sleep_schedule[99][2].sleep_duration == 10min);
    }

    SECTION("Total Sleep Time")
    {
        using namespace std::literals::chrono_literals;

        auto const schedule = parseInput(sample_input);
        auto sleep_schedule = calculateSleepSchedule(schedule);

        CHECK(totalSleepTimeForGuard(sleep_schedule[10]) == 50min);
        CHECK(totalSleepTimeForGuard(sleep_schedule[99]) == 30min);
    }

    SECTION("Find Guard With Most Sleep")
    {
        auto const schedule = parseInput(sample_input);
        auto sleep_schedule = calculateSleepSchedule(schedule);

        CHECK(findGuardWithMostSleep(sleep_schedule) == 10);
    }

    SECTION("Find Sleepiest Minute")
    {
        auto const schedule = parseInput(sample_input);
        auto sleep_schedule = calculateSleepSchedule(schedule);

        CHECK(findSleepiestMinute(sleep_schedule[10]) == 24);
        CHECK(findSleepiestMinute(sleep_schedule[99]) == 45);
    }

    SECTION("Strategy 1")
    {
        auto const schedule = parseInput(sample_input);
        auto sleep_schedule = calculateSleepSchedule(schedule);

        CHECK(calculateStrategy1(sleep_schedule) == 240);
    }
}
