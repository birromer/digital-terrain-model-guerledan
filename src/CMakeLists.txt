cmake_minimum_required(VERSION 3.5)
project(src)

set(cmake_build_type debug)
set(cmake_cxx_flags "-Wall -Wextra -std=c++11")

include_directories(${PROJ_INCLUDE_DIRS})
find_package(PROJ)

add_executable(create_raster
    main.cpp
    mesh/Mesh.cpp
)

target_link_libraries(create_raster /usr/lib/libproj.so ${PROJ_LIBRARIES})
