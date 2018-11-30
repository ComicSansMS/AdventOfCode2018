add_library(01_lib STATIC lib.hpp lib.cpp)
target_include_directories(01_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_lib)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_lib_test lib.t.cpp)
    target_link_libraries(01_lib_test PUBLIC Catch 01_lib)
    add_test(NAME TestAdvent01 COMMAND 01_lib_test)
    enable_coverage(01_lib)
endif()
