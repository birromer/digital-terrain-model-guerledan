cmake_minimum_required(VERSION 3.5)
project(src)

set(cmake_build_type debug)
set(cmake_cxx_flags "-Wall -Wextra -std=c++11")

add_executable(create_raster
    main.cpp
)
