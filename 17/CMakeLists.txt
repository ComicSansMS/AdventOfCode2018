add_library(17_water_simulation STATIC water_simulation.hpp water_simulation.cpp)
target_include_directories(17_water_simulation PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(17_water_simulation PUBLIC date range-v3)
add_executable(advent17 advent17.cpp)
target_link_libraries(advent17 PUBLIC 17_water_simulation)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(17_water_simulation_test water_simulation.t.cpp)
    target_link_libraries(17_water_simulation_test PUBLIC Catch 17_water_simulation)
    add_test(NAME TestAdvent17 COMMAND 17_water_simulation_test)
    enable_coverage(17_water_simulation)
endif()
