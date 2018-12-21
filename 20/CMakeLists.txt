add_library(20_regex_labyrinth STATIC regex_labyrinth.hpp regex_labyrinth.cpp)
target_include_directories(20_regex_labyrinth PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(20_regex_labyrinth PUBLIC date range-v3)
add_executable(advent20 advent20.cpp)
target_link_libraries(advent20 PUBLIC 20_regex_labyrinth)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(20_regex_labyrinth_test regex_labyrinth.t.cpp)
    target_link_libraries(20_regex_labyrinth_test PUBLIC Catch 20_regex_labyrinth)
    add_test(NAME TestAdvent20 COMMAND 20_regex_labyrinth_test)
    enable_coverage(20_regex_labyrinth)
endif()
