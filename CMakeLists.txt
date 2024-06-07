cmake_minimum_required(VERSION 3.20)
project(collectives)
find_package(MPI REQUIRED)

add_executable(bcast bcast.c)
target_link_libraries(bcast MPI::MPI_C)
