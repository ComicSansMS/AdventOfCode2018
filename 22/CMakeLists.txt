add_library(22_spelunking STATIC spelunking.hpp spelunking.cpp)
target_include_directories(22_spelunking PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(22_spelunking PUBLIC date range-v3)
add_executable(advent22 advent22.cpp)
target_link_libraries(advent22 PUBLIC 22_spelunking)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(22_spelunking_test spelunking.t.cpp)
    target_link_libraries(22_spelunking_test PUBLIC Catch 22_spelunking)
    add_test(NAME TestAdvent22 COMMAND 22_spelunking_test)
    enable_coverage(22_spelunking)
endif()
