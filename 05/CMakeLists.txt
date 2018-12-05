add_library(05_polymer STATIC polymer.hpp polymer.cpp)
target_include_directories(05_polymer PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(05_polymer PUBLIC date range-v3)
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC 05_polymer)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(05_polymer_test polymer.t.cpp)
    target_link_libraries(05_polymer_test PUBLIC Catch 05_polymer)
    add_test(NAME TestAdvent05 COMMAND 05_polymer_test)
    enable_coverage(05_polymer)
endif()
