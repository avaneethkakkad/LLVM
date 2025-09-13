cmake_minimum_required(VERSION 3.13)
project(ConstantFolding LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(ConstantFolding MODULE ConstantFolding.cpp)

set_target_properties(ConstantFolding PROPERTIES
    CXX_STANDARD 17
    COMPILE_FLAGS "-fno-rtti"
)

#llvm_map_components_to_libnames(llvm_libs core analysis)
#target_link_libraries(ConstantFolding PRIVATE ${llvm_libs})

