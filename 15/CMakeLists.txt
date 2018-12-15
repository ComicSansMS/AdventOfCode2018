add_library(15_combat_simulator STATIC combat_simulator.hpp combat_simulator.cpp)
target_include_directories(15_combat_simulator PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(15_combat_simulator PUBLIC date range-v3)
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC 15_combat_simulator)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(15_combat_simulator_test combat_simulator.t.cpp)
    target_link_libraries(15_combat_simulator_test PUBLIC Catch 15_combat_simulator)
    add_test(NAME TestAdvent15 COMMAND 15_combat_simulator_test)
    enable_coverage(15_combat_simulator)
endif()
