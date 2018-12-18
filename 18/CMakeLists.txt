add_library(18_game_of_woods STATIC game_of_woods.hpp game_of_woods.cpp)
target_include_directories(18_game_of_woods PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(18_game_of_woods PUBLIC date range-v3)
add_executable(advent18 advent18.cpp)
target_link_libraries(advent18 PUBLIC 18_game_of_woods)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(18_game_of_woods_test game_of_woods.t.cpp)
    target_link_libraries(18_game_of_woods_test PUBLIC Catch 18_game_of_woods)
    add_test(NAME TestAdvent18 COMMAND 18_game_of_woods_test)
    enable_coverage(18_game_of_woods)
endif()
