add_library(16_assembly_reverse STATIC assembly_reverse.hpp assembly_reverse.cpp)
target_include_directories(16_assembly_reverse PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(16_assembly_reverse PUBLIC date range-v3)
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC 16_assembly_reverse)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(16_assembly_reverse_test assembly_reverse.t.cpp)
    target_link_libraries(16_assembly_reverse_test PUBLIC Catch 16_assembly_reverse)
    add_test(NAME TestAdvent16 COMMAND 16_assembly_reverse_test)
    enable_coverage(16_assembly_reverse)
endif()
