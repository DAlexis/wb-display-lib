cmake_minimum_required(VERSION 2.8)

project("wbdl")

set(LIB_SOURCE
    wbdl.cpp
)

set(LIB_HPP
    wbdl.hpp
    wbdl-driver.hpp
    wbdl-base.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE} ${LIB_HPP})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_rvalue_references)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_lambdas)

export_include_dirs(
    ${PROJECT_SOURCE_DIR}
    ${${PROJECT_NAME}_USED_INCDIRS}
)

export_libs(
    ${PROJECT_NAME}
)
