add_library(23_nanobots STATIC nanobots.hpp nanobots.cpp)
target_include_directories(23_nanobots PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(23_nanobots PUBLIC date range-v3)
add_executable(advent23 advent23.cpp)
target_link_libraries(advent23 PUBLIC 23_nanobots)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(23_nanobots_test nanobots.t.cpp)
    target_link_libraries(23_nanobots_test PUBLIC Catch 23_nanobots)
    add_test(NAME TestAdvent23 COMMAND 23_nanobots_test)
    enable_coverage(23_nanobots)
endif()
