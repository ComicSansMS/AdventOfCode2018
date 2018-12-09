add_library(09_marble_game STATIC marble_game.hpp marble_game.cpp)
target_include_directories(09_marble_game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(09_marble_game PUBLIC date range-v3)
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC 09_marble_game)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(09_marble_game_test marble_game.t.cpp)
    target_link_libraries(09_marble_game_test PUBLIC Catch 09_marble_game)
    add_test(NAME TestAdvent09 COMMAND 09_marble_game_test)
    enable_coverage(09_marble_game)
endif()
