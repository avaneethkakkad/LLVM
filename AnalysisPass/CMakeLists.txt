cmake_minimum_required(VERSION 3.13)
project(SimpleAnalysis)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(SimpleAnalysis MODULE SimpleAnalysis.cpp)

set_target_properties(SimpleAnalysis PROPERTIES
    CXX_STANDARD 17
    COMPILE_FLAGS "-fno-rtti"
)

