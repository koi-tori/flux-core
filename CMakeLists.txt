cmake_minimum_required(VERSION 3.20)

project(FLUXC LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINATIONS})

llvm_map_components_to_libnames(llvm_libs core support irreader)

add_executable(fluxc src/fluxc.cpp src/fcore.cpp src/fast.cpp src/ftokenizer.cpp)
target_link_libraries(fluxc PRIVATE ${llvm_libs})
