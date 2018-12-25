add_library(25_spacetime STATIC spacetime.hpp spacetime.cpp)
target_include_directories(25_spacetime PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(25_spacetime PUBLIC date range-v3)
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC 25_spacetime)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(25_spacetime_test spacetime.t.cpp)
    target_link_libraries(25_spacetime_test PUBLIC Catch 25_spacetime)
    add_test(NAME TestAdvent25 COMMAND 25_spacetime_test)
    enable_coverage(25_spacetime)
endif()
