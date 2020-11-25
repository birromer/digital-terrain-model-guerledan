cmake_minimum_required(VERSION 3.5)
project(src)

set(cmake_build_type debug)
set(cmake_cxx_flags "-Wall -Wextra -std=c++11")

find_package(PROJ)
#find_package(OpenCV REQUIRED)

include_directories(${PROJ_INCLUDE_DIRS})
#include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(create_raster
    main.cpp
    mesh/Mesh.cpp
)

target_link_libraries(create_raster ${PROJ_LIBRARIES})
#target_link_libraries(create_raster ${OpenCV_LIBS})
