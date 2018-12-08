add_library(08_license_tree STATIC license_tree.hpp license_tree.cpp)
target_include_directories(08_license_tree PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(08_license_tree PUBLIC date range-v3)
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC 08_license_tree)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(08_license_tree_test license_tree.t.cpp)
    target_link_libraries(08_license_tree_test PUBLIC Catch 08_license_tree)
    add_test(NAME TestAdvent08 COMMAND 08_license_tree_test)
    enable_coverage(08_license_tree)
endif()
