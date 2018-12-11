add_library(11_fuel_cell_grid STATIC fuel_cell_grid.hpp fuel_cell_grid.cpp)
target_include_directories(11_fuel_cell_grid PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(11_fuel_cell_grid PUBLIC date range-v3)
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC 11_fuel_cell_grid)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(11_fuel_cell_grid_test fuel_cell_grid.t.cpp)
    target_link_libraries(11_fuel_cell_grid_test PUBLIC Catch 11_fuel_cell_grid)
    add_test(NAME TestAdvent11 COMMAND 11_fuel_cell_grid_test)
    enable_coverage(11_fuel_cell_grid)
endif()
