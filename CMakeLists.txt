cmake_minimum_required(VERSION 3.16)

project(bignum-arithmetics LANGUAGES CXX)

add_subdirectory(long_arithmetics)

add_executable(main main.cpp)
target_link_libraries(main long_arithmetics)

add_executable(pi pi.cpp)
target_link_libraries(pi long_arithmetics)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(tests tests.cpp)
target_link_libraries(tests long_arithmetics gtest gtest_main)