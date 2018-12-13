add_library(12_plant_spreading STATIC plant_spreading.hpp plant_spreading.cpp)
target_include_directories(12_plant_spreading PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(12_plant_spreading PUBLIC date range-v3)
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC 12_plant_spreading)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(12_plant_spreading_test plant_spreading.t.cpp)
    target_link_libraries(12_plant_spreading_test PUBLIC Catch 12_plant_spreading)
    add_test(NAME TestAdvent12 COMMAND 12_plant_spreading_test)
    enable_coverage(12_plant_spreading)
endif()
