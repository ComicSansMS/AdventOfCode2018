add_library(06_manhattan_area STATIC manhattan_area.hpp manhattan_area.cpp)
target_include_directories(06_manhattan_area PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_manhattan_area PUBLIC date range-v3)
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC 06_manhattan_area)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(06_manhattan_area_test manhattan_area.t.cpp)
    target_link_libraries(06_manhattan_area_test PUBLIC Catch 06_manhattan_area)
    add_test(NAME TestAdvent06 COMMAND 06_manhattan_area_test)
    enable_coverage(06_manhattan_area)
endif()
