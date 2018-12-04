add_library(04_guard_schedule STATIC guard_schedule.hpp guard_schedule.cpp)
target_include_directories(04_guard_schedule PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(04_guard_schedule PUBLIC date range-v3)
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC 04_guard_schedule)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(04_guard_schedule_test guard_schedule.t.cpp)
    target_link_libraries(04_guard_schedule_test PUBLIC Catch 04_guard_schedule)
    add_test(NAME TestAdvent04 COMMAND 04_guard_schedule_test)
    enable_coverage(04_guard_schedule)
endif()
