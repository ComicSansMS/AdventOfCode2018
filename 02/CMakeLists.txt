add_library(02_warehouse_ids STATIC warehouse_ids.hpp warehouse_ids.cpp)
target_include_directories(02_warehouse_ids PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02_warehouse_ids PUBLIC range-v3)
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC 02_warehouse_ids)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(02_warehouse_ids_test warehouse_ids.t.cpp)
    target_link_libraries(02_warehouse_ids_test PUBLIC Catch 02_warehouse_ids)
    add_test(NAME TestAdvent02 COMMAND 02_warehouse_ids_test)
    enable_coverage(02_warehouse_ids)
endif()
