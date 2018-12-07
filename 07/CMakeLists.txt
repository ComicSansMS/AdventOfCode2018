add_library(07_sleigh_assembly STATIC sleigh_assembly.hpp sleigh_assembly.cpp)
target_include_directories(07_sleigh_assembly PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(07_sleigh_assembly PUBLIC date range-v3)
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC 07_sleigh_assembly)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(07_sleigh_assembly_test sleigh_assembly.t.cpp)
    target_link_libraries(07_sleigh_assembly_test PUBLIC Catch 07_sleigh_assembly)
    add_test(NAME TestAdvent07 COMMAND 07_sleigh_assembly_test)
    enable_coverage(07_sleigh_assembly)
endif()
