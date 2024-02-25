cmake_minimum_required(VERSION 3.16)

project(bignum-arithmetics LANGUAGES CXX)

add_subdirectory(long_arithmetics)
# add_subdirectory(pi)
# add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main long_arithmetics)