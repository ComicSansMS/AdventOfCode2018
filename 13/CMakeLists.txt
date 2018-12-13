add_library(13_mine_cart_madness STATIC mine_cart_madness.hpp mine_cart_madness.cpp)
target_include_directories(13_mine_cart_madness PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(13_mine_cart_madness PUBLIC date range-v3)
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC 13_mine_cart_madness)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(13_mine_cart_madness_test mine_cart_madness.t.cpp)
    target_link_libraries(13_mine_cart_madness_test PUBLIC Catch 13_mine_cart_madness)
    add_test(NAME TestAdvent13 COMMAND 13_mine_cart_madness_test)
    enable_coverage(13_mine_cart_madness)
endif()
