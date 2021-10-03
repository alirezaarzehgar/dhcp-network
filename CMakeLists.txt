cmake_minimum_required(VERSION 3.0)

project(dhcp-network)

add_library(${PROJECT_NAME}
    src/listener.c
)

add_library(lib::dhcpnetwork ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
lib::pkt
)

add_subdirectory(dhcp-pkt)

add_subdirectory(tests)