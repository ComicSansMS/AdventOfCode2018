add_library(21_register_machine_ip STATIC register_machine_ip.hpp register_machine_ip.cpp)
target_include_directories(21_register_machine_ip PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(21_register_machine_ip PUBLIC date range-v3)
add_executable(advent21 advent21.cpp)
target_link_libraries(advent21 PUBLIC 21_register_machine_ip)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
