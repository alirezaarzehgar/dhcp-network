cmake_minimum_required(VERSION 3.0)

project(dhcp-network-tests)

add_executable(${PROJECT_NAME}
    network_test.c
    main.c
    listener_test.c
)

target_link_libraries(${PROJECT_NAME}
    cunit
    lib::dhcpnetwork
)
