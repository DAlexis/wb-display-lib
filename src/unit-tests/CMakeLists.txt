cmake_minimum_required(VERSION 2.8)

project(wbdl-unit-tests)

find_package (GTest REQUIRED)

include_directories (
    ${GTEST_INCLUDE_DIR}
    ${wbdl_INCLUDE_DIRS}
)

set(EXE_SOURCES
    wbdl-base.cpp
)

set(EXE_HPP
    
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES} ${EXE_HPP})

target_link_libraries (${PROJECT_NAME}
    gtest
    gtest_main
    ${wbdl_LIBRARY}
)

add_test(NAME Library_testing
         COMMAND ${PROJECT_NAME})

add_unit_test(${PROJECT_NAME})
