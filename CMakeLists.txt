cmake_minimum_required(VERSION 3.10)
project(BlockchainSuite)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)
add_subdirectory(src)
add_executable(BlockchainSuite src/main.cpp)
target_link_libraries(BlockchainSuite cases suite)