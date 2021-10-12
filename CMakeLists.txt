cmake_minimum_required(VERSION 3.6)

set(PROJECT_NAME dhcp-network)
set(DHCP_PKT dhcp-pkt)
set(DHCP_NETWORK_TESTS tests)

project(${PROJECT_NAME})

find_package(Git QUIET)

find_path(CUNIT_INCLUDE_DIR NAMES CUnit/CUnit.h)

find_library(CUNIT_LIBRARY NAMES
    cunit
    libcunit
    cunitlib
)

if(NOT CUNIT_INCLUDE_DIR)
    message(FATAL_ERROR "Cunit header files not found")
endif()

if(NOT CUNIT_LIBRARY)
    message(FATAL_ERROR "Cunit library not found")
endif()

message("\n--------------------------------\n")
message("PROJECT NAME:\t\t${PROJECT_NAME}")
message("CMAKE_SYSTEM_NAME:\t${CMAKE_SYSTEM_NAME}")
message("CMAKE_C_COMPILER:\t${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:\t${CMAKE_CXX_COMPILER}")
message("CMAKE_BINARY_DIR:\t${CMAKE_BINARY_DIR}")
message("CUNIT_LIBRARY:\t\t${CUNIT_LIBRARY}")
message("\n--------------------------------\n")

if(GIT_FOUND AND EXISTS "../.git")
    OPTION(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT)

        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

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

add_subdirectory(${DHCP_PKT})
add_subdirectory(${DHCP_NETWORK_TESTS})