add_library(03_rectangle_cover STATIC rectangle_cover.hpp rectangle_cover.cpp)
target_include_directories(03_rectangle_cover PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_rectangle_cover PUBLIC range-v3)
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC 03_rectangle_cover)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(03_rectangle_cover_test rectangle_cover.t.cpp)
    target_link_libraries(03_rectangle_cover_test PUBLIC Catch 03_rectangle_cover)
    add_test(NAME TestAdvent03 COMMAND 03_rectangle_cover_test)
    enable_coverage(03_rectangle_cover)
endif()
