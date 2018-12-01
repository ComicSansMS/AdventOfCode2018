add_library(01_calibration STATIC calibration.hpp calibration.cpp)
target_include_directories(01_calibration PUBLIC ${CMAKE_CURRENT_LIST_DIR})
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_calibration)

file(COPY input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_calibration_test calibration.t.cpp)
    target_link_libraries(01_calibration_test PUBLIC Catch 01_calibration)
    add_test(NAME TestAdvent01 COMMAND 01_calibration_test)
    enable_coverage(01_calibration)
endif()
