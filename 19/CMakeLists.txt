add_library(19_register_machine_ip STATIC register_machine_ip.hpp register_machine_ip.cpp)
target_include_directories(19_register_machine_ip PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(19_register_machine_ip PUBLIC date range-v3)
add_executable(advent19 advent19.cpp)
target_link_libraries(advent19 PUBLIC 19_register_machine_ip)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(19_register_machine_ip_test register_machine_ip.t.cpp)
    target_link_libraries(19_register_machine_ip_test PUBLIC Catch 19_register_machine_ip)
    add_test(NAME TestAdvent19 COMMAND 19_register_machine_ip_test)
    enable_coverage(19_register_machine_ip)
endif()
