add_library(24_bacteria_battle STATIC bacteria_battle.hpp bacteria_battle.cpp)
target_include_directories(24_bacteria_battle PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(24_bacteria_battle PUBLIC date range-v3)
add_executable(advent24 advent24.cpp)
target_link_libraries(advent24 PUBLIC 24_bacteria_battle)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(24_bacteria_battle_test bacteria_battle.t.cpp)
    target_link_libraries(24_bacteria_battle_test PUBLIC Catch 24_bacteria_battle)
    add_test(NAME TestAdvent24 COMMAND 24_bacteria_battle_test)
    enable_coverage(24_bacteria_battle)
endif()
