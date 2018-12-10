add_library(10_moving_stars STATIC moving_stars.hpp moving_stars.cpp)
target_include_directories(10_moving_stars PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(10_moving_stars PUBLIC date range-v3)
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC 10_moving_stars)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(10_moving_stars_test moving_stars.t.cpp)
    target_link_libraries(10_moving_stars_test PUBLIC Catch 10_moving_stars)
    add_test(NAME TestAdvent10 COMMAND 10_moving_stars_test)
    enable_coverage(10_moving_stars)
endif()
