cmake_minimum_required(VERSION 3.5)
project(tests)

enable_testing()

#set(cmake_build_type debug)
set(cmake_cxx_flags "-Wall -Wextra -std=c++11")

add_executable(tests
#    ../src/Cell.cpp
#    ../src/Path.cpp
#    ../src/vibes.cpp
    tests_main.cpp
)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

target_link_libraries(tests Catch2::Catch2)

include(CTest)

#include(./catch2/Catch.cmake)
#catch_discover_tests(tests)

include(./catch2/ParseAndAddCatchTests.cmake)
ParseAndAddCatchTests(tests)
