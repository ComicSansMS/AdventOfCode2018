add_library(25_adventures_in_spacetime STATIC adventures_in_spacetime.hpp adventures_in_spacetime.cpp)
target_include_directories(25_adventures_in_spacetime PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(25_adventures_in_spacetime PUBLIC date range-v3)
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC 25_adventures_in_spacetime)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(25_adventures_in_spacetime_test adventures_in_spacetime.t.cpp)
    target_link_libraries(25_adventures_in_spacetime_test PUBLIC Catch 25_adventures_in_spacetime)
    add_test(NAME TestAdvent25 COMMAND 25_adventures_in_spacetime_test)
    enable_coverage(25_adventures_in_spacetime)
endif()
