file (GLOB_RECURSE SOURCES "src/*.cpp")
add_library(long_arithmetics SHARED ${SOURCES})

target_include_directories(long_arithmetics PUBLIC include)