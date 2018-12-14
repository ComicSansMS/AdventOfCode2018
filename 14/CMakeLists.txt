add_library(14_hot_chocolate STATIC hot_chocolate.hpp hot_chocolate.cpp)
target_include_directories(14_hot_chocolate PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(14_hot_chocolate PUBLIC date range-v3)
add_executable(advent14 advent14.cpp)
target_link_libraries(advent14 PUBLIC 14_hot_chocolate)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(14_hot_chocolate_test hot_chocolate.t.cpp)
    target_link_libraries(14_hot_chocolate_test PUBLIC Catch 14_hot_chocolate)
    add_test(NAME TestAdvent14 COMMAND 14_hot_chocolate_test)
    enable_coverage(14_hot_chocolate)
endif()
