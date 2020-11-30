cmake_minimum_required(VERSION 3.5)
project(dtm-guerledan)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(create_raster ${PROJ_LIBRARIES})
